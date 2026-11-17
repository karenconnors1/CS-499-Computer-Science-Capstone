"""Offline tests for CS 499 database-side filtering; no live MongoDB required."""
import importlib.util
from pathlib import Path
import sys
import types
import unittest

try:
    import pymongo
except ImportError:
    pymongo = types.ModuleType("pymongo")
    pymongo.MongoClient = object  # Offline tests inject a fake collection.
    errors = types.ModuleType("pymongo.errors")
    errors.PyMongoError = type("PyMongoError", (Exception,), {})
    pymongo.errors = errors
    sys.modules["pymongo"] = pymongo
    sys.modules["pymongo.errors"] = errors

module_path = Path(__file__).resolve().parents[1] / "Enhanced" / "animal_shelter_enhanced.py"
spec = importlib.util.spec_from_file_location("animal_shelter_enhanced", module_path)
mod = importlib.util.module_from_spec(spec)
spec.loader.exec_module(mod)


class RecordingCollection:
    def __init__(self):
        self.calls = []
    def find(self, query, projection=None):
        self.calls.append((query, projection))
        return [{"animal_type": "Dog", "breed": "Labrador Retriever Mix"}]


class RescueQueryTests(unittest.TestCase):
    def test_all_categories_use_database_query(self):
        for category, age_range, sex in (
            ("water", (26, 156), "Intact Female"),
            ("mountain", (26, 156), "Intact Male"),
            ("disaster", (20, 300), "Intact Male"),
        ):
            with self.subTest(category=category):
                collection = RecordingCollection()
                shelter = mod.AnimalShelter(collection=collection)
                shelter.filterData(category)
                self.assertEqual(len(collection.calls), 1)
                query = collection.calls[0][0]
                self.assertEqual(query["animal_type"], "Dog")
                self.assertEqual(query["sex_upon_outcome"], sex)
                self.assertEqual(query["age_upon_outcome_in_weeks"],
                                 {"$gte": age_range[0], "$lte": age_range[1]})
                self.assertEqual(query["breed"]["$in"], mod.RESCUE_CRITERIA[category]["breeds"])

    def test_all_and_normalized_categories(self):
        collection = RecordingCollection()
        shelter = mod.AnimalShelter(collection=collection)
        shelter.filterData("all")
        self.assertEqual(collection.calls[-1][0], {})
        shelter.filterData(" WATER ")
        self.assertEqual(collection.calls[-1][0]["sex_upon_outcome"], "Intact Female")

    def test_invalid_type_cannot_trigger_unfiltered_read(self):
        collection = RecordingCollection()
        shelter = mod.AnimalShelter(collection=collection)
        for invalid in ("oops", "", None, 4):
            with self.subTest(invalid=invalid), self.assertRaises(ValueError):
                shelter.filterData(invalid)
        self.assertEqual(collection.calls, [])

    def test_rejects_unbounded_update_and_delete(self):
        shelter = mod.AnimalShelter(collection=RecordingCollection())
        with self.assertRaises(ValueError):
            shelter.delete({})
        with self.assertRaises(ValueError):
            shelter.update({}, {"name": "New"})
        with self.assertRaises(ValueError):
            shelter.update({"name": "Old"}, {"$unset": 1})

    def test_read_requires_dictionary(self):
        shelter = mod.AnimalShelter(collection=RecordingCollection())
        with self.assertRaises(ValueError):
            shelter.read(None)


if __name__ == "__main__":
    unittest.main()
