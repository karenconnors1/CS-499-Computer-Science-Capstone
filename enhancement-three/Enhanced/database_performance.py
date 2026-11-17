
"""
CS 499 Milestone Four - Enhancement Three: Databases
Grazioso Salvare Animal Rescue Dashboard

Database Performance Evaluation

Compares:
1. Original full-collection retrieval
2. Filtered queries using a forced collection scan
3. Filtered queries using the compound index
4. Filtered queries with MongoDB choosing the query plan

Uses a local MongoDB database containing synthetic test data.
This script does not create indexes or modify animal records.
"""

import json
from pymongo import MongoClient
from animal_shelter_enhanced import build_rescue_query


MONGO_URI = "mongodb://127.0.0.1:27017/"
DATABASE_NAME = "CS499_AnimalRescue_Performance"
COLLECTION_NAME = "animals"
INDEX_NAME = "rescue_filter_compound_idx"


def explain_query(collection, query, hint=None):
    """Retrieve MongoDB execution statistics for a query."""

    find_command = {
        "find": collection.name,
        "filter": query,
    }

    if hint is not None:
        find_command["hint"] = hint

    return collection.database.command(
        "explain",
        find_command,
        verbosity="executionStats",
    )


def extract_statistics(explanation):
    """Extract important performance metrics."""

    stats = explanation.get("executionStats", {})

    return {
        "documents_returned": stats.get("nReturned"),
        "execution_time_ms": stats.get("executionTimeMillis"),
        "documents_examined": stats.get("totalDocsExamined"),
        "index_keys_examined": stats.get("totalKeysExamined"),
    }


def compare_queries(collection, rescue_type):
    """Compare original retrieval and enhanced database filtering."""

    print("\n" + "=" * 60)
    print("RESCUE CATEGORY:", rescue_type.upper())
    print("=" * 60)

    original_query = {}
    enhanced_query = build_rescue_query(rescue_type)

    print("\nEnhanced MongoDB query:")
    print(json.dumps(enhanced_query, indent=2))

    # Original dashboard retrieved the entire collection.
    original_stats = extract_statistics(
        explain_query(
            collection,
            original_query,
            hint={"$natural": 1},
        )
    )

    # Enhanced filtering, without using the compound index.
    scan_stats = extract_statistics(
        explain_query(
            collection,
            enhanced_query,
            hint={"$natural": 1},
        )
    )

    # Enhanced filtering, explicitly using the compound index.
    indexed_stats = extract_statistics(
        explain_query(
            collection,
            enhanced_query,
            hint=INDEX_NAME,
        )
    )

    # Let MongoDB decide which query plan to use.
    automatic_stats = extract_statistics(
        explain_query(collection, enhanced_query)
    )

    print("\nOriginal full-collection retrieval:")
    print(json.dumps(original_stats, indent=2))

    print("\nEnhanced filtering - collection scan:")
    print(json.dumps(scan_stats, indent=2))

    print("\nEnhanced filtering - compound index:")
    print(json.dumps(indexed_stats, indent=2))

    print("\nEnhanced filtering - automatic selection:")
    print(json.dumps(automatic_stats, indent=2))

    scanned = scan_stats["documents_examined"]
    indexed = indexed_stats["documents_examined"]

    if scanned and indexed is not None:
        reduction = ((scanned - indexed) / scanned) * 100
        print(
            "\nReduction in documents examined: "
            f"{reduction:.2f}%"
        )

    if (
        scan_stats["documents_returned"]
        != indexed_stats["documents_returned"]
    ):
        raise AssertionError(
            f"Result counts differ for {rescue_type}"
        )

    print("\nResult-count consistency: PASS")

    return {
        "category": rescue_type,
        "original": original_stats,
        "filtered_collection_scan": scan_stats,
        "filtered_compound_index": indexed_stats,
        "filtered_automatic": automatic_stats,
    }


def main():
    print("CS 499 Database Performance Evaluation")
    print("Dataset: Local synthetic animal records")

    client = MongoClient(
        MONGO_URI,
        serverSelectionTimeoutMS=5000,
    )

    try:
        client.admin.command("ping")

        collection = client[DATABASE_NAME][COLLECTION_NAME]

        total = collection.count_documents({})

        print("\nMongoDB connection established.")
        print("Database:", DATABASE_NAME)
        print("Collection:", COLLECTION_NAME)
        print("Documents:", total)

        if total == 0:
            raise RuntimeError(
                "Performance collection is empty. "
                "Generate the synthetic test data first."
            )

        available_indexes = [
            index["name"]
            for index in collection.list_indexes()
        ]

        print("\nCurrent indexes:", available_indexes)

        if INDEX_NAME not in available_indexes:
            raise RuntimeError(
                "Required compound index is missing."
            )

        for rescue_type in (
            "water",
            "mountain",
            "disaster",
        ):
            compare_queries(collection, rescue_type)

        print("\nEvaluation completed successfully.")
        print(
            "Note: Query timings are individual MongoDB "
            "explain measurements, not dashboard response times."
        )

    finally:
        client.close()


if __name__ == "__main__":
    try:
        main()
    except Exception as error:
        print("\nPerformance evaluation could not run:")
        print(type(error).__name__ + ": " + str(error))
        raise
