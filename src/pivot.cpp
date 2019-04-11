#include "pivot.h"

int pivot::getPivot(enhancedgraph *g, int color)
{
    TIntH *colorMap = g->colors;
    for (THashKeyDatI<TInt, TInt> i = colorMap->BegI(); i < colorMap->EndI(); i++)
    {
        if (i.GetDat()==color)
        {
            return i.GetKey();
        }
        
    }
    return -1;
};