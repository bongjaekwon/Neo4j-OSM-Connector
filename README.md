# Neo4j-OSM-Connector

Neo4j OSM Importer is a simple GUI application designed to load OpenStreetMap (OSM) data into a Neo4j database. Built using Qt, this program utilizes Podman to run a Neo4j container and executes Cypher queries to import OSM data.

---

## Features

1. **Start Podman Container**  
   Launch a Neo4j 3.5.2 container to set up the database environment.

2. **Import OSM Data to Neo4j**  
   Import OSM data from a specified local file path into Neo4j.

3. **Execute Cypher Queries via REST API**  
   Send Cypher queries to Neo4j using its REST API for data processing.

---

## System Requirements

- **Operating System**: Linux (e.g., openSUSE, Fedora)
- **Dependencies**:
  - Qt5/Qt6 (including Qt Widgets)
  - Podman
  - Neo4j 3.5.2
  - libcurl

---

## Build and Run

### 1. **Install Dependencies**

- **Install Qt Development Environment**
  ```bash
  sudo zypper install qt5-devel  # openSUSE
  sudo dnf install qt5-qtbase-devel  # Fedora

## Usage

### Start the Podman Container
1. Enter the OSM file path in the input field.
2. Click the `Start Podman Container` button to launch the Neo4j container.
3. The status bar will indicate whether the container is running.

### Import OSM Data into Neo4j
1. Click the `Import OSM to Neo4j` button to execute a Cypher query and import the OSM data.

---

## Troubleshooting

### Podman Errors
- Verify that Podman is correctly installed.
- Use `sudo` if additional permissions are required to run Podman commands.

### Cypher Query Issues
- Ensure Neo4j's REST API is running and accessible.
- Confirm that the Basic Authentication credentials are correct.

---

## Future Improvements
- Add validation for the OSM file path.
- Support additional Cypher query options.
- Enhance container management and status monitoring.

---

## License
This project is licensed under the MIT License.


