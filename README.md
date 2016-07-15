# vault
Vault is a generalized data recording and saving utility with strong type information that plays well with IDEs and compilers.
Vault is for data organizing and exporting, and is well-suited for computational simulation data collection.

## Example
Here is an example program using Vault to collect and save information.
```c++
#include <iostream>

#define FIELDS \
FIELD( int, update ) \
FIELD_STATIC( std::string, experiment )
#include "vault/vault.h"

int main(int argc, char* argv[]) {
	for (int i=0; i<55; i++) {
		auto rec = VAULT::newRecord(); // type is VAULT::Record*
		rec->update = i;
	}
	VAULT::Record::experiment = "test_run";
	VAULT::save("results.ssv");
	return(0);
}
```

Output
```bash
$ head results.ssv | column -t
update  experiment
0       test_run
1       test_run
2       test_run
3       test_run
4       test_run
5       test_run
6       test_run
7       test_run
8       test_run
```

## Usage

###1. Download into your project

```bash
git clone https://github.com/JorySchossau/vault.git vault
```

### 2. Define fields and include the header
 In your c++ project, define your fields of interest in an early preprocessed area (such as the top of a main.cpp). Vault handles any type, include your own classes, as long as they override the ostream<< operator for serializing to text. FIELD is for data which is possibly unique to each record, whereas FIELD_STATIC is for data unique to the data recording session, such as the experimental condition.

```c++
#define FIELDS \
FIELD(int, update) \
FIELD(float, fitness) \
FIELD_STATIC(std::string, condition) \
FIELD_STATIC(int, replicate)
#include "vault/vault.h" // must be last
```

### 3. Collect data
Simply ask Vault for a new record each time you're ready to record new data, or if you are still adding data you can get the last record.

```c++
auto record = VAULT::newRecord(); // get a new record
record->update = i++;
record->fitness = getFitness();
...
// some other function
auto record = VAULT::back();
record.fitness = fmax(1.0, record.fitness);
```

### 4. Save to file
When you're ready to save data to a file, use the save() function. Record any information general to the entire dataset, such as experimental condition, or maybe replicate number.

```c++
VAULT::Record::condition = "test_simulation";
VAULT::Record::replicate = 9;
VAULT::save("test_simulation.ssv"); // space-separated file
```

---
## To Do

 - Multiple simultaneous Record instances saving to different files
 - Allow flushes to disk to reduce memory footprint.
 - Configurable value separator (default is space).
