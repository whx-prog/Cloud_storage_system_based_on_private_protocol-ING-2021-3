#include "factory.h"
int hash(char *key) {
    int h = 0, g;
    while (*key)
    {
        h = (h << 4) + *key++;
        g = h & 0xf0000000;
        if (g)
            h ^= g >> 24;
        h &= ~g;

    }
    return h % MAXKEY;

}
void hashInsert(int hashNode[],phashNode_t node,int num) {
    int hashVal = hash(node->word);
    if (hashNode[hashVal] == 0) {
        switch(num)
        {
        case 1:
            hashNode[hashVal]=1;
            break;
        case 2:
            hashNode[hashVal]=2;
            break;
        case 3:
            hashNode[hashVal]=3;
            break;
        case 4:
            hashNode[hashVal]=4;
            break;
        case 5:
            hashNode[hashVal]=5;
            break;
        case 6:
            hashNode[hashVal]=6;
            break;
        case 7:
            hashNode[hashVal]=7;
            break;
        case 8:
            hashNode[hashVal]=8;
            break;
        case 9:
            hashNode[hashVal]=9;
            break;
        case 10:
            hashNode[hashVal]=10;
            break;
        case 11:
            hashNode[hashVal]=11;
            break;
        case 12:
            hashNode[hashVal]=12;
            break;
        }
        printf("hasval=%d,and flag is %d\n",hashNode[hashVal],hashVal);
    }
}
/*void Findhash(phashNode_t *hashNode, phashNode_t node) {
    int hashVal = hash(node->word);
        phashNode_t pCur = hashNode[hashVal];
        while (pCur != NULL) {
            if (strcmp(node->word, pCur->word) == 0) {
                node->token = pCur->token; 
                break;
            }
            pCur = pCur->next;

        }
        node->token=NOT;
}
*/
