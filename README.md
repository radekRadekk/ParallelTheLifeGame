### Parallel The Life Game

- build with *make*
- run *Life*
- params:

| Parameter                        | Flag           | Values           |
| -------------------------------- |:--------------:| ----------------:|
| Run mode (sequential or prallel) | -mode          | SEQ / PAR        |
| Number of iterations             | -i             | e.g. 1000        |
| Size of board                    | -size          | e.g. 1000        |
| Number of threads                | -t             | e.g. 4           |
| Name of output file              | -out           | e.g. example.gif |
| Data fragmentation method        | -frag          | SQU / LIN        |

**Example run:** 

./Life -mode PAR -i 500 -size 1000 -t 8 -out result.gif -frag SQU

---

- Name of output file must have *.gif* extension.
- You can clear workspace with *make clean*.
