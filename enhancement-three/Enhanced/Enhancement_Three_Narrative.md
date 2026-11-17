# CS 499 Capstone — Enhancement Three: Databases

**Artifact:** Grazioso Salvare Animal Rescue Dashboard
**Original Course:** CS 340 — Client/Server Development
**Enhancement Category:** Databases

## Artifact Description and Justification

For my third capstone enhancement, I selected the Grazioso Salvare Animal Rescue Dashboard from CS 340: Client/Server Development. The original artifact combined a Python CRUD module, MongoDB, and an interactive dashboard to identify shelter animals suitable for specialized rescue training.

I selected this artifact because it demonstrates how database design, query efficiency, data validation, and application integration affect software quality.

## Database Enhancements

I enhanced the AnimalShelter CRUD module by implementing MongoDB-side filtering for Water Rescue, Mountain/Wilderness Rescue, and Disaster/Individual Tracking. The queries use animal type, breed, sex upon outcome, and age to identify qualifying records.

I added input validation and safeguards against unrestricted database update and delete operations. I also created a compound MongoDB index on animal type, sex upon outcome, breed, and age.

The dashboard was updated to use the enhanced CRUD module, avoid hardcoded database credentials, handle missing geographic data, and run with modern Dash.

## Performance Evaluation

I generated 10,000 synthetic animal records in a local MongoDB database to evaluate the enhancements.

MongoDB execution-plan measurements showed the following reductions in documents examined:

- Water Rescue: 10,000 to 292 documents (97.08% reduction).
- Mountain/Wilderness Rescue: 10,000 to 975 documents (90.25% reduction).
- Disaster/Individual Tracking: 10,000 to 2,061 documents (79.39% reduction).

All three result-count consistency checks passed.

These results demonstrate more selective database retrieval. The recorded timings measure individual MongoDB query execution plans, not complete dashboard response times.

## Testing and Validation

Eight automated Python unit tests passed. They covered category normalization, database-query usage, invalid filter inputs, read input validation, and safeguards against unrestricted update and delete operations.

I also tested the enhanced dashboard with the synthetic MongoDB dataset. Its four callbacks registered successfully, and the rescue dropdown returned the expected counts for all categories.

Browser testing confirmed that the animal table and breed distribution chart updated when rescue categories changed. The mapping feature handled records without location coordinates without assigning inaccurate markers.

## Challenges and Lessons Learned

One challenge was adapting the original JupyterDash dashboard to a modern Dash environment. Another was evaluating database behavior without relying on the original remote academic database.

Creating a local synthetic dataset provided a repeatable way to test query behavior and evaluate MongoDB execution plans.

This work reinforced the value of separating database operations from presentation logic, validating inputs, and measuring improvements rather than relying on assumptions.

## Reflection and Professional Growth

The enhancement strengthened my skills in MongoDB query design, compound indexing, Python application integration, defensive programming, and testing.

It demonstrates my ability to evaluate an existing artifact, implement database-focused improvements, and verify those improvements through automated tests and measurable technical evidence.
