# ReedSalomon
*By [@dabecart](https://www.dabecart.net/en/).*

This is my own C implementation of the Reed-Salomon Error Correction Algorithm. 

In particular, I've developed this algorithm **to recover the FLASH memory of an critical embedded system if it somehow gets corrupted**. The microcontroller will have a verification step before running its own software. If the verification fails, it will use the reparation data (or *Extra Points* as its called in code) stored on an external EEPROM to repair the damaged data blocks. Once repaired, it will allow the microcontroller to run normally. 

The code is highly customizable (check [CommonDefines.h](/src/CommonDefines.h)). Using the parameters set at the time of writing, my approach has the following characteristics:

- For every block of 10 bytes of original data, 4 bytes are added as recuperation data. That's a data efficiency of 40%. 
- It can repair with ~99.97% accuracy a 10 byte block with up to 2 corrupted bytes at ~9600 bytes/sec.
- It can detect if a block has more than two errors and is thus irreparable with ~99.78% accuracy.
- This is kinda dumb to specify it, but it can check block without errors with 100% accuracy at ~79000 bytes/sec (~77 kB/sec).
- The algorithm speed can be quite variable depending on the number of errors found on blocks. The lesser the errors, the better the algorithm runs. Using the benchmark function `testBench()` with random data blocks of `rand[0,3]` errors, the average speed seems to be around 2500 bytes/sec on my not so beefy Windows computer running *WSL*.
- You can also specify to the algorithm if the **EEPROM is corrupted or not** (`EEPROM_NOT_CORRUPTED` in [CommonDefines.h](/src/CommonDefines.h)), meaning the recuperation data may not be reliable. If it were to be corrupted, the algorithm can still operate with the same conditions as before suffering a little in reliability (~91.4% with same testcase as before) and speed (~1300 bytes/sec).

It's a really simple to understand algorithm that gets a little bit cloudy once you enter the modular arithmetic realm. Nevertheless, it's quite interesting to learn about it, and I'm honestly quite impressed with how I got to optimize the algorithm by using math rather than code. 

# Compiling, running and testing it

The project has its own [Makefile](/Makefile) to build the project. Simply run:

```
$ make
```

To run on Linux:

```
$ ./reed
```

To clean the build files:

```
$ make clean
```

# Components of the algorithm

## Encoder

The **encoder** adds a set of extra points to every chunk of data (a chunk is a combination of data blocks). These extra points can be used to *repair* the data if it somehow gets corrupted. The original data and the extra points are stored on separated files (as one will be stored on the FLASH of the micro and the other on the external EEPROM).

These extra points are a combination of points from the Reed-Salomon original approach and one more byte that is a combination of a Hamming code with a small CRC of the data block with those previous extra points. This final byte has two purposes and it's only used when the EEPROM isn't corrupted:

1. In the case there's only one byte that's gone bad, the Hamming code will point directly to which one it was.
2. The CRC is a simple tool to verify that the correction done to the block was OK. In the case it failed, it would *try again* (make a different combination of points to the error correction algorithm) till the CRC matches.

## VeriRecover

The **verifier** will receive both files, the original (that could be corrupted) data and the reparation data, and will verify first if the data is OK. That is done using a combination of CRCs added per chunk on the recuperation data. If the CRC of the chunk is OK, then it will be skipped (this is configurable). If the CRC were to be wrong, then the **recover** enters into action and tries to fix the chunk. It will iterate through the compounding data blocks and will fix them one by one. Once done, it will check if the CRC of the chunk is OK. If not, it can be configured to do a thorough recovery (this function is still not implemented as I think it may be too expensive to compute and there's not much of a gain to be obtained by implementing it).

# The basis of the algorithm

