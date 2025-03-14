//This is a with preallocation of memory demo of idea of ag85aor1 ordering by Arthur Golubev 1985.
//Note: to keep the demo simpler reapting values are intentianally not multiple in the resulting set

#include <climits>
#include <cstdint>
#include <iostream>
#include <stack>
#include <deque>

struct AG85DST1InUnderlyingStorageUnit{
    size_t zeroNextUnitIndex = 0;
    size_t oneNextUnitIndex = 0;
};

int main(){

    uint8_t inputSequence[] = {8, 13, 5, 8, 8, 2, 1, 0};

    size_t inputSequenceLength = (sizeof(inputSequence) / sizeof(inputSequence[0]));
    std::cout << "Input sequence (" << inputSequenceLength << " items):";
    for (
        size_t printInputSeqNumbersCounter = 0
        ; printInputSeqNumbersCounter < inputSequenceLength
        ; ++printInputSeqNumbersCounter)
    {
        std::cout << " " << (int)(inputSequence[printInputSeqNumbersCounter]);
    }
    std::cout << std::endl;

    uint8_t resultSequence[inputSequenceLength];

    std::deque<AG85DST1InUnderlyingStorageUnit> ag85dst1UnderlyingStorage;
    size_t currAG85DST1UnderlyingStorageFreePlaceIndex = 1;
    
    size_t ag85dst1StartingUnderlyingStorageIndex = 0;

    size_t bitsItemLength = sizeof(uint8_t) * CHAR_BIT;
    
    uint8_t bitsMask;

    for (uint8_t number : inputSequence){
        size_t currUnderlyingStorageIndex = ag85dst1StartingUnderlyingStorageIndex;
        bitsMask = 1 << (bitsItemLength - 1);
        for (size_t bitsInNumberCounter = 0; bitsInNumberCounter < bitsItemLength; ++bitsInNumberCounter){
            if (! (number & bitsMask)) {
                if (0 == ag85dst1UnderlyingStorage[currUnderlyingStorageIndex].zeroNextUnitIndex){
                    if((ag85dst1UnderlyingStorage.size() - 1) < currUnderlyingStorageIndex){
                        ag85dst1UnderlyingStorage.emplace_back();
                    }
                    ag85dst1UnderlyingStorage[currUnderlyingStorageIndex].zeroNextUnitIndex
                        = currAG85DST1UnderlyingStorageFreePlaceIndex;
                    ++currAG85DST1UnderlyingStorageFreePlaceIndex;
                }
                currUnderlyingStorageIndex 
                    = ag85dst1UnderlyingStorage[currUnderlyingStorageIndex].zeroNextUnitIndex;
            } else {
                if((ag85dst1UnderlyingStorage.size() - 1) < currUnderlyingStorageIndex){
                    ag85dst1UnderlyingStorage.emplace_back();
                }
                if (0 == ag85dst1UnderlyingStorage[currUnderlyingStorageIndex].oneNextUnitIndex){
                    ag85dst1UnderlyingStorage[currUnderlyingStorageIndex].oneNextUnitIndex
                        = currAG85DST1UnderlyingStorageFreePlaceIndex;
                    ++currAG85DST1UnderlyingStorageFreePlaceIndex;
                }
                currUnderlyingStorageIndex 
                    = ag85dst1UnderlyingStorage[currUnderlyingStorageIndex].oneNextUnitIndex;
            }
            bitsMask = bitsMask >> 1;
        }
    }

    struct TraversalStackItem{
        uint8_t valueTraversedPiece;
        uint8_t nextTraverseBitsMask;
        size_t ag85dst1UnderlyingStorageIndex;
        TraversalStackItem(
            uint8_t setValueTraversedPiece
            , uint8_t setTraverseBitsMask
            , size_t setAG85DST1UnderlyingStorageIndex
            )
        : valueTraversedPiece(setValueTraversedPiece)
        , nextTraverseBitsMask(setTraverseBitsMask)
        , ag85dst1UnderlyingStorageIndex(setAG85DST1UnderlyingStorageIndex)
        {            
        }
    };

    std::stack<TraversalStackItem> treeLinksPtrsStack;
    uint8_t currTraverseBitsMask = (1 << (bitsItemLength - 1));
    if (0 != ag85dst1UnderlyingStorage[ag85dst1StartingUnderlyingStorageIndex].oneNextUnitIndex){
        treeLinksPtrsStack.emplace(
            currTraverseBitsMask
            , (currTraverseBitsMask >> 1)
            , ag85dst1UnderlyingStorage[ag85dst1StartingUnderlyingStorageIndex].oneNextUnitIndex
        );
    }
    if (0 != ag85dst1UnderlyingStorage[ag85dst1StartingUnderlyingStorageIndex].zeroNextUnitIndex){
        treeLinksPtrsStack.emplace(
            0
            , (currTraverseBitsMask >> 1)
            , ag85dst1UnderlyingStorage[ag85dst1StartingUnderlyingStorageIndex].zeroNextUnitIndex
        );
    }
    
    size_t traversedNumbersCounter = 0;

    while(!treeLinksPtrsStack.empty()){
        
        uint8_t valueTraversedPiece = treeLinksPtrsStack.top().valueTraversedPiece;
        currTraverseBitsMask = treeLinksPtrsStack.top().nextTraverseBitsMask;
        size_t currTraverseUnitIndex = treeLinksPtrsStack.top().ag85dst1UnderlyingStorageIndex;

        treeLinksPtrsStack.pop();

        if (currTraverseBitsMask == 0){

            resultSequence[traversedNumbersCounter] = valueTraversedPiece;
            ++traversedNumbersCounter;

        } else {

            if (ag85dst1UnderlyingStorage[currTraverseUnitIndex].oneNextUnitIndex){
                
                treeLinksPtrsStack.emplace(
                    (valueTraversedPiece | currTraverseBitsMask)
                    , currTraverseBitsMask >> 1
                    , ag85dst1UnderlyingStorage[currTraverseUnitIndex].oneNextUnitIndex
                );
            }
            if (ag85dst1UnderlyingStorage[currTraverseUnitIndex].zeroNextUnitIndex){

                treeLinksPtrsStack.emplace(
                    valueTraversedPiece
                    , currTraverseBitsMask >> 1
                    , ag85dst1UnderlyingStorage[currTraverseUnitIndex].zeroNextUnitIndex
                );
            }

        }

    }

    std::cout << "Without repeatings result (" << traversedNumbersCounter << " items):";
    for (
        size_t printResultSeqNumbersCounter = 0
        ; printResultSeqNumbersCounter < traversedNumbersCounter
        ; ++printResultSeqNumbersCounter)
    {
        std::cout << " " << (int)(resultSequence[printResultSeqNumbersCounter]);
    }
    std::cout << std::endl;

    return 0;
}