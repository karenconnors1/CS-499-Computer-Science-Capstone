"""Additional offline edge-case tests for CS 499 Enhancement Three."""

import unittest
from test_step2_filtering import mod


class EmptyCollection:
    def find(self, query, projection=None):
        return []


class FailingCollection:
    def find(self, query, projection=None):
        raise mod.PyMongoError("Simulated database failure")


class DatabaseEdgeCaseTests(unittest.TestCase):

    def test_empty_query_results(self):
        shelter = mod.AnimalShelter(collection=EmptyCollection())
        self.assertEqual(shelter.filterData("water"), [])

    def test_database_read_failure(self):
        shelter = mod.AnimalShelter(collection=FailingCollection())

        with self.assertRaisesRegex(
            RuntimeError, "MongoDB read operation failed"
        ):
            shelter.filterData("mountain")

    def test_invalid_projection(self):
        shelter = mod.AnimalShelter(collection=EmptyCollection())

        with self.assertRaises(ValueError):
            shelter.read({}, projection="invalid")


if __name__ == "__main__":
    unittest.main()
