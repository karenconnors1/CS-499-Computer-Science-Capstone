# Grazioso Salvare Animal Rescue Dashboard
## CS 499 Capstone — Milestone Four: Enhancement Three (Databases)

**Original course:** CS 340 — Client/Server Development
**Enhancement category:** Databases

### Project Overview

The Grazioso Salvare Animal Rescue Dashboard is a Python, MongoDB, and Dash application for identifying animal shelter records that meet specialized rescue-training criteria.

The original CS 340 application provided MongoDB CRUD operations and an interactive dashboard. This CS 499 enhancement emphasizes efficient database queries, defensive input validation, indexing, performance evaluation, and testing.

### Original and Enhanced Artifacts

- `Project Two/animal_shelter.py` — Original CRUD module.
- `Project Two/ProjectTwoDashboard.ipynb` — Original dashboard.
- `Enhanced/animal_shelter_enhanced.py` — Enhanced CRUD module and MongoDB-side rescue filtering.
- `Enhanced/ProjectTwoDashboard_Enhanced.ipynb` — Updated interactive dashboard.
- `Enhanced/generate_test_data.py` — Synthetic data generator.
- `Enhanced/database_performance.py` — MongoDB query performance evaluation.
- `Enhanced/performance_results.txt` — Recorded performance evidence.
- `Tests/test_step2_filtering.py` — Query validation and defensive-programming tests.
- `Tests/test_step3_edge_cases.py` — Empty-result and database-error tests.

### Enhancements Implemented

1. Moved rescue-category filtering into MongoDB.
2. Added validation for rescue categories and CRUD inputs.
3. Added safeguards against unrestricted update and delete operations.
4. Supported credentials supplied through environment variables instead of hardcoded credentials in the enhanced CRUD module.
5. Added dependency injection for offline unit testing.
6. Evaluated a compound index using MongoDB execution plans.
7. Updated the interactive dashboard to use the enhanced database module and handle missing geographic data.

### Requirements

- Python 3.10 or compatible Python 3 version.
- MongoDB running locally at `127.0.0.1:27017`.
- PyMongo 4.18.3 for the tested environment.
- Dash, dash-leaflet, pandas, plotly, Jupyter Notebook, matplotlib, and numpy for the dashboard.

From the project root, install Python dependencies into the active virtual environment:

```bash
python3 -m pip install "pymongo==4.18.3" dash dash-leaflet pandas plotly notebook matplotlib numpy
```

Use a virtual environment where possible. Package installation and application behavior may differ across operating systems and library versions.

### Generate the Synthetic Test Data

Start the local MongoDB server before running these commands.

From the project root:

```bash
python3 Enhanced/generate_test_data.py
```

The generator uses:

- Database: `CS499_AnimalRescue_Performance`
- Collection: `animals`
- Expected records: 10,000

The generator skips insertion if the collection already contains records. It does not modify the original CS 340 dataset.

**Important:** These records are synthetic and intended for educational performance testing. They do not represent actual shelter animals.

### Create the Compound MongoDB Index

The performance evaluation requires an index named `rescue_filter_compound_idx`.

Create it once using:

```bash
python3 - <<'INDEX_SETUP'
from pymongo import MongoClient, ASCENDING

client = MongoClient("mongodb://127.0.0.1:27017")

collection = client["CS499_AnimalRescue_Performance"]["animals"]

name = collection.create_index(
    [
        ("animal_type", ASCENDING),
        ("sex_upon_outcome", ASCENDING),
        ("breed", ASCENDING),
        ("age_upon_outcome_in_weeks", ASCENDING),
    ],
    name="rescue_filter_compound_idx",
)

print("Index ready:", name)
client.close()
INDEX_SETUP
```

### Run the Automated Tests

From the project root:

```bash
python3 -m unittest discover -s Tests -p "test_*.py" -v
```

The verified test suite contains eight tests covering rescue-query construction, category normalization, invalid input, unrestricted modification prevention, empty results, simulated database errors, and projection validation.

All eight tests passed during development.

An additional simulated-collection check confirmed that the CRUD module can retrieve records with missing fields. This does not establish that every possible malformed record can be displayed in every dashboard visualization.

### Run the Performance Evaluation

After generating the synthetic data and creating the compound index, execute:

```bash
python3 Enhanced/database_performance.py
```

The script compares full-collection retrieval, filtered collection scans, indexed queries, and automatic query-plan selection.

Recorded results from 10,000 synthetic records:

| Rescue Category | Full Retrieval Documents Examined | Indexed Documents Examined | Reduction |
|---|---:|---:|---:|
| Water Rescue | 10,000 | 292 | 97.08% |
| Mountain/Wilderness Rescue | 10,000 | 975 | 90.25% |
| Disaster/Individual Tracking | 10,000 | 2,061 | 79.39% |

All three result-count consistency checks passed.

These percentages represent reductions in documents examined. Individual MongoDB execution-plan timings are not complete dashboard response-time measurements.

### Launch the Enhanced Dashboard

Start the local MongoDB server and ensure the synthetic dataset is available.

Install the dashboard dependencies, then open the enhanced notebook:

```bash
python3 -m notebook "Enhanced/ProjectTwoDashboard_Enhanced.ipynb"
```

In Jupyter, select **Kernel > Restart Kernel and Run All Cells** if necessary.

When the Dash server starts, open:

`http://127.0.0.1:8050/`

The dashboard supports Reset/All, Water Rescue, Mountain/Wilderness Rescue, and Disaster/Individual Tracking.

In browser testing, all four filtering options returned the expected record counts and updated the data table and breed distribution chart.

### Security and Design Considerations

The enhanced CRUD module validates inputs, rejects unsupported rescue categories, and prevents update and delete operations using empty query dictionaries. These safeguards reduce the risk of unintended database operations.

The local synthetic test environment uses a direct MongoDB connection. It is an educational testing configuration, not a production security model. Production deployment would require appropriate authentication, authorization, secure configuration, network restrictions, and additional security testing.

### Known Limitations

- The performance dataset is synthetic.
- Generated records do not contain geographic coordinates, so the map may display a message indicating that location data is unavailable.
- The original Grazioso Salvare logo image is not included.
- Performance measurements describe database execution plans rather than end-to-end application latency.
- The local database is not embedded in the submission ZIP; the included generator recreates the synthetic records.
- The original CS 340 artifact is preserved for comparison and may depend on its historical academic environment.

### Verification Summary

- Eight automated unit tests passed.
- Missing-field retrieval passed in a simulated-collection test.
- Local MongoDB connection passed.
- All four dashboard filtering options were visually tested.
- Three database performance consistency checks passed.
- Original and enhanced artifacts are preserved separately.

### Academic Context

This enhancement was completed for Southern New Hampshire University's CS 499 Computer Science Capstone, Milestone Four: Enhancement Three — Databases. The professional enhancement narrative is submitted separately as a Microsoft Word document.
