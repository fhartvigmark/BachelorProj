#include "scc.h"

int scc::FindSCCs(PNGraph g, int trimlevel, int pivotmethod, int fwbwmethod){
    switch (fwbwmethod){
        case 0:
            return fwbw::basicFWBW(g, trimlevel, pivotmethod);
    };
};
