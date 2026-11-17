
"""CS 499 Milestone Four: Enhancement Three - Databases

Artifact: Grazioso Salvare Animal Rescue Dashboard
Original Course: CS 340 - Client/Server Development

Enhancements:
- Perform rescue-category filtering directly in MongoDB.
- Preserve the original water, mountain, and disaster criteria.
- Validate rescue categories and database operation inputs.
- Read database credentials from environment variables.
- Prevent unrestricted update and delete operations.
- Allow dependency injection for offline unit testing.

Indexing and performance evaluation will be added separately.
"""

import os
from urllib.parse import quote_plus

from pymongo import MongoClient
from pymongo.errors import PyMongoError


# ---------------------------------------------------------
# Rescue filtering criteria from the original CS 340 project
# ---------------------------------------------------------

RESCUE_CRITERIA = {
    "water": {
        "breeds": [
            "Labrador Retriever Mix",
            "Chesapeake Bay Retriever",
            "Newfoundland"
        ],
        "sex": "Intact Female",
        "min_weeks": 26,
        "max_weeks": 156
    },

    "mountain": {
        "breeds": [
            "German Shepherd",
            "Alaskan Malamute",
            "Old English Sheepdog",
            "Siberian Husky",
            "Rottweiler"
        ],
        "sex": "Intact Male",
        "min_weeks": 26,
        "max_weeks": 156
    },

    "disaster": {
        "breeds": [
            "Doberman Pinscher",
            "German Shepherd",
            "Golden Retriever",
            "Bloodhound",
            "Rottweiler"
        ],
        "sex": "Intact Male",
        "min_weeks": 20,
        "max_weeks": 300
    }
}


# ---------------------------------------------------------
# Build MongoDB rescue queries
# ---------------------------------------------------------

def build_rescue_query(rescue_type):
    """Build a MongoDB query for a supported rescue category.

    Valid categories:
        water
        mountain
        disaster
        all

    Invalid categories raise ValueError rather than
    unexpectedly retrieving the entire collection.
    """

    if not isinstance(rescue_type, str):
        raise ValueError(
            "Rescue type must be a string."
        )

    selected = rescue_type.strip().lower()

    if selected == "all":
        return {}

    if selected not in RESCUE_CRITERIA:
        raise ValueError(
            "Invalid rescue type. Select water, mountain, "
            "disaster, or all."
        )

    criteria = RESCUE_CRITERIA[selected]

    return {
        "animal_type": "Dog",

        "breed": {
            "$in": list(criteria["breeds"])
        },

        "sex_upon_outcome": criteria["sex"],

        "age_upon_outcome_in_weeks": {
            "$gte": criteria["min_weeks"],
            "$lte": criteria["max_weeks"]
        }
    }


# ---------------------------------------------------------
# AnimalShelter database class
# ---------------------------------------------------------

class AnimalShelter:
    """CRUD operations and MongoDB-side rescue filtering.

    Uses the AAC MongoDB database and animals collection.
    """

    # -----------------------------------------------------
    # Constructor: establish MongoDB connection
    # -----------------------------------------------------

    def __init__(
        self,
        username=None,
        password=None,
        *,
        collection=None,
        host=None,
        port=None
    ):

        self.client = None
        self.database = None

        # Allow a mock collection for offline unit tests.
        # No MongoDB connection is needed when testing.
        if collection is not None:
            self.collection = collection
            return

        # Read credentials from environment variables
        # unless explicitly supplied by the caller.
        username = (
            username
            if username is not None
            else os.environ.get("AAC_MONGO_USERNAME")
        )

        password = (
            password
            if password is not None
            else os.environ.get("AAC_MONGO_PASSWORD")
        )

        if not username or not password:
            raise ValueError(
                "MongoDB credentials are required. "
                "Set AAC_MONGO_USERNAME and "
                "AAC_MONGO_PASSWORD."
            )

        # Preserve original SNHU database location.
        host = host or os.environ.get(
            "AAC_MONGO_HOST",
            "nv-desktop-services.apporto.com"
        )

        port = int(
            port or os.environ.get(
                "AAC_MONGO_PORT",
                "34547"
            )
        )

        # Encode special characters in credentials.
        mongo_uri = (
            f"mongodb://{quote_plus(username)}:"
            f"{quote_plus(password)}@"
            f"{host}:{port}/AAC"
        )

        # Create MongoDB client.
        self.client = MongoClient(
            mongo_uri,
            authSource="admin",
            serverSelectionTimeoutMS=5000
        )

        # Access database and collection.
        self.database = self.client["AAC"]
        self.collection = self.database["animals"]

    # -----------------------------------------------------
    # CREATE: insert an animal record
    # -----------------------------------------------------

    def create(self, data):
        """Insert a new animal record if it does not exist."""

        if not isinstance(data, dict) or not data:
            raise ValueError(
                "Create data must be a non-empty dictionary."
            )

        try:
            # Preserve original duplicate prevention.
            existing = self.collection.find_one(data)

            if existing is not None:
                return False

            result = self.collection.insert_one(data)

            return bool(result.acknowledged)

        except PyMongoError as error:
            raise RuntimeError(
                "MongoDB create operation failed."
            ) from error

    # -----------------------------------------------------
    # READ: retrieve records using a MongoDB query
    # -----------------------------------------------------

    def read(self, query, projection=None):
        """Retrieve matching documents directly from MongoDB.

        An empty dictionary intentionally retrieves all
        records. None and other invalid types are rejected.

        A projection can optionally limit returned fields.
        """

        if not isinstance(query, dict):
            raise ValueError(
                "Read query must be a dictionary."
            )

        if projection is not None and not isinstance(
            projection, dict
        ):
            raise ValueError(
                "Projection must be a dictionary or None."
            )

        try:
            results = self.collection.find(
                query,
                projection
            )

            return list(results)

        except PyMongoError as error:
            raise RuntimeError(
                "MongoDB read operation failed."
            ) from error

    # -----------------------------------------------------
    # UPDATE: modify existing records
    # -----------------------------------------------------

    def update(
        self,
        query,
        update_values,
        multiple=False
    ):
        """Update one or multiple matching records safely."""

        # Prevent unrestricted modifications.
        if not isinstance(query, dict) or not query:
            raise ValueError(
                "Update query must be a non-empty dictionary."
            )

        if (
            not isinstance(update_values, dict)
            or not update_values
        ):
            raise ValueError(
                "Update values must be a non-empty dictionary."
            )

        # Reject MongoDB operators and dotted field paths.
        for key in update_values:
            if str(key).startswith("$") or "." in str(key):
                raise ValueError(
                    "Update field names cannot contain "
                    "operators or dots."
                )

        try:
            if multiple:
                result = self.collection.update_many(
                    query,
                    {"$set": update_values}
                )
            else:
                result = self.collection.update_one(
                    query,
                    {"$set": update_values}
                )

            return result.modified_count

        except PyMongoError as error:
            raise RuntimeError(
                "MongoDB update operation failed."
            ) from error

    # -----------------------------------------------------
    # DELETE: remove existing records
    # -----------------------------------------------------

    def delete(self, query, multiple=False):
        """Delete matching records using a validated query."""

        # Prevent accidental deletion of all records.
        if not isinstance(query, dict) or not query:
            raise ValueError(
                "Delete query must be a non-empty dictionary."
            )

        try:
            if multiple:
                result = self.collection.delete_many(
                    query
                )
            else:
                result = self.collection.delete_one(
                    query
                )

            return result.deleted_count

        except PyMongoError as error:
            raise RuntimeError(
                "MongoDB delete operation failed."
            ) from error

    # -----------------------------------------------------
    # FILTER: retrieve animals by rescue category
    # -----------------------------------------------------

    def filterData(self, rescue_type):
        """Filter rescue candidates directly in MongoDB.

        Unlike the original application, this method
        does not retrieve every record and then filter
        the collection using Python list comprehensions.
        """

        query = build_rescue_query(rescue_type)

        return self.read(query)

    # -----------------------------------------------------
    # CLOSE: release MongoDB client resources
    # -----------------------------------------------------

    def close(self):
        """Close the MongoDB client connection."""

        if self.client is not None:
            self.client.close()
