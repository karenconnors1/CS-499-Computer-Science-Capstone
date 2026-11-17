# Import MongoClient to connect to MongoDB
from pymongo import MongoClient

# Import ObjectId to reference document IDs in MongoDB (not currently used but useful for future enhancements)
from bson.objectid import ObjectId

# Define the AnimalShelter class which handles all CRUD operations on the AAC MongoDB collection
class AnimalShelter:
    """CRUD operations for the Animal collection in MongoDB using aac_shelter_outcomes.csv data"""

    # -------------------------------------
    # Constructor: establishes DB connection
    # -------------------------------------
    def __init__(self, username, password):
        # Create a MongoDB client with authentication credentials
        self.client = MongoClient(
            'mongodb://%s:%s@nv-desktop-services.apporto.com:34547/AAC' % (username, password),
            authSource='admin'  # Specifies which database to authenticate against
        )
        # Access the AAC database
        self.database = self.client['AAC']
        # Access the 'animals' collection containing animal outcome data
        self.collection = self.database['animals']

    # -------------------------------------
    # CREATE operation: insert a new document
    # -------------------------------------
    def create(self, data):
        # Check if input is a non-empty dictionary
        if isinstance(data, dict) and data:
            try:
                # Check if document already exists to prevent duplicates
                if self.database.animals.find_one(data):
                    print("Duplicate entry found. Insert skipped.")
                    return False
                else:
                    # Insert the new document and return the success status
                    result = self.database.animals.insert_one(data)
                    return result.acknowledged  # Returns True if insertion was successful
            except Exception as e:
                # Raise exception if insertion fails
                raise Exception(f"An error occurred while inserting data: {str(e)}")
        else:
            # Raise error for invalid input
            raise Exception("Invalid data: must be a non-empty dictionary")

    # -------------------------------------
    # READ operation: retrieve documents based on a query
    # -------------------------------------
    def read(self, query):
        # Ensure the query is not None
        if query is not None:
            # Return all matching documents as a list
            return list(self.database.animals.find(query))
        else:
            # Raise an error if the query is empty
            raise Exception("Nothing to read, because query parameter is empty")

    # -------------------------------------
    # UPDATE operation: update existing documents
    # -------------------------------------
    def update(self, query, update_values, multiple=False):
        # Ensure query and update data are both dictionaries
        if isinstance(query, dict) and isinstance(update_values, dict):
            try:
                # Update multiple documents if multiple=True, otherwise update only one
                if multiple:
                    result = self.database.animals.update_many(query, {'$set': update_values})
                else:
                    result = self.database.animals.update_one(query, {'$set': update_values})
                # Return the number of documents updated
                return result.modified_count
            except Exception as e:
                # Raise error if the update fails
                raise Exception(f"An error occurred while updating data: {str(e)}")
        else:
            # Raise error for invalid input
            raise Exception("Invalid input: query and update_values must be dictionaries")

    # -------------------------------------
    # DELETE operation: delete documents based on a query
    # -------------------------------------
    def delete(self, query, multiple=False):
        # Ensure the query is a dictionary
        if isinstance(query, dict):
            try:
                # Delete multiple documents if multiple=True, otherwise delete only one
                if multiple:
                    result = self.database.animals.delete_many(query)
                else:
                    result = self.database.animals.delete_one(query)
                # Return the number of documents deleted
                return result.deleted_count
            except Exception as e:
                # Raise error if the deletion fails
                raise Exception(f"An error occurred while deleting data: {str(e)}")
        else:
            # Raise error for invalid input
            raise Exception("Invalid input: query must be a dictionary")

    # -------------------------------------
    # FILTER operation: apply custom filters based on rescue type
    # -------------------------------------
    def filterData(self, rescue_type):
        """
        Filters animal data based on rescue type using pre-defined criteria.

        Args:
            rescue_type (str): One of 'water', 'mountain', 'disaster', or 'all'.

        Returns:
            list: Filtered list of documents matching the rescue type criteria.
        """
        # Read all data from the collection
        data = self.read({})

        # Water rescue filter: specific breeds, female, 6 months to 3 years
        if rescue_type == 'water':
            return [
                d for d in data
                if d.get('animal_type') == 'Dog' and
                   d.get('breed') in ['Labrador Retriever Mix', 'Chesapeake Bay Retriever', 'Newfoundland'] and
                   d.get('sex_upon_outcome') == 'Intact Female' and
                   26 <= d.get('age_upon_outcome_in_weeks', 0) <= 156
            ]

        # Mountain rescue filter: specific breeds, male, 6 months to 3 years
        elif rescue_type == 'mountain':
            return [
                d for d in data
                if d.get('animal_type') == 'Dog' and
                   d.get('breed') in ['German Shepherd', 'Alaskan Malamute', 'Old English Sheepdog', 'Siberian Husky', 'Rottweiler'] and
                   d.get('sex_upon_outcome') == 'Intact Male' and
                   26 <= d.get('age_upon_outcome_in_weeks', 0) <= 156
            ]

        # Disaster rescue filter: specific breeds, male, 5 months to almost 6 years
        elif rescue_type == 'disaster':
            return [
                d for d in data
                if d.get('animal_type') == 'Dog' and
                   d.get('breed') in ['Doberman Pinscher', 'German Shepherd', 'Golden Retriever', 'Bloodhound', 'Rottweiler'] and
                   d.get('sex_upon_outcome') == 'Intact Male' and
                   20 <= d.get('age_upon_outcome_in_weeks', 0) <= 300
            ]

        # If rescue_type is 'all' or unrecognized, return all data
        return data
