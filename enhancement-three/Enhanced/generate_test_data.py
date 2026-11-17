
"""
CS 499 Milestone Four - Enhancement Three: Databases
Generate synthetic animal shelter records for local performance testing.

Writes ONLY to the CS499_AnimalRescue_Performance database.
Does not modify the original CS 340 dataset.
"""

from pymongo import MongoClient

DATABASE_NAME = "CS499_AnimalRescue_Performance"
COLLECTION_NAME = "animals"
TOTAL_RECORDS = 10000

BREEDS = [
    "Labrador Retriever Mix",
    "Chesapeake Bay Retriever",
    "Newfoundland",
    "German Shepherd",
    "Alaskan Malamute",
    "Old English Sheepdog",
    "Siberian Husky",
    "Rottweiler",
    "Doberman Pinscher",
    "Golden Retriever",
    "Bloodhound",
    "Beagle",
    "Domestic Shorthair",
]

def main():
    client = MongoClient(
        "mongodb://127.0.0.1:27017/",
        serverSelectionTimeoutMS=5000,
    )

    try:
        client.admin.command("ping")
        collection = client[DATABASE_NAME][COLLECTION_NAME]

        if collection.estimated_document_count() > 0:
            print("Performance collection already contains records.")
            print("No changes made to avoid duplicates.")
            return

        records = []

        for number in range(TOTAL_RECORDS):
            breed = BREEDS[number % len(BREEDS)]
            animal_type = "Cat" if breed == "Domestic Shorthair" else "Dog"

            record = {
                "animal_id": f"PERF-{number:05d}",
                "animal_type": animal_type,
                "breed": breed,
                "sex_upon_outcome": (
                    "Intact Female" if number % 3 == 0
                    else "Intact Male"
                ),
                "age_upon_outcome_in_weeks": 10 + (number % 350),
                "name": f"Test Animal {number}",
            }

            records.append(record)

        collection.insert_many(records)

        print("Database:", DATABASE_NAME)
        print("Collection:", COLLECTION_NAME)
        print("Records inserted:", len(records))
        print("Total documents:", collection.count_documents({}))

    finally:
        client.close()


if __name__ == "__main__":
    main()
