                if (pCeldas->status==SANO && pCeldas->vacuna==FALSO && pCeldas->cubrebocas==FALSO)
                    printf("S ");
                if (pCeldas->status==SANO && pCeldas->vacuna==FALSO && pCeldas->cubrebocas==VERDADERO)
                    printf("s ");
                if (pCeldas->status==SANO && pCeldas->vacuna==VERDADERO && pCeldas->cubrebocas==FALSO)
                    printf("V ");
                if (pCeldas->status==SANO && pCeldas->vacuna==VERDADERO && pCeldas->cubrebocas==VERDADERO)
                    printf("v ");
                if (pCeldas->status==ENFERMO  && pCeldas->cubrebocas==FALSO)
                    printf("E ");
                if (pCeldas->status==ENFERMO && pCeldas->cubrebocas==VERDADERO)
                    printf("e ");
                if (pCeldas->status==RECUPERADO)
                    printf("r ");