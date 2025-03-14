//This is a demo of idea of ag85aor1 ordering by Arthur Golubev 1985.
//Note: to keep the demo simpler reapting values are intentianally not multiple in the resulting set

#include <climits>
#include <cstdint>
#include <iostream>
#include <stack>

struct AG85DST1Link{
    AG85DST1Link * zeroNextLinkPtr = nullptr;
    AG85DST1Link * oneNextLinkPtr = nullptr;
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

    uint8_t resultSequence[(sizeof(inputSequence) / sizeof(inputSequence[0]))];
    
    AG85DST1Link * treeRoot = new AG85DST1Link;

    size_t bitsItemLength = sizeof(uint8_t) * CHAR_BIT;
    
    uint8_t bitsMask;

    for (uint8_t number : inputSequence){
        AG85DST1Link * currParrentPtr = treeRoot;
        bitsMask = 1 << (bitsItemLength - 1);
        for (size_t bitsInNumberCounter = 0; bitsInNumberCounter < bitsItemLength; ++bitsInNumberCounter){
            if (! (number & bitsMask)) {
                if (! currParrentPtr->zeroNextLinkPtr) {
                    currParrentPtr->zeroNextLinkPtr = new AG85DST1Link;
                }
                currParrentPtr = currParrentPtr->zeroNextLinkPtr;
            } else {
                if (! currParrentPtr->oneNextLinkPtr) {
                    currParrentPtr->oneNextLinkPtr = new AG85DST1Link;
                }
                currParrentPtr = currParrentPtr->oneNextLinkPtr;
            }
            bitsMask = bitsMask >> 1;
        }
    }

    struct TraversalStackItem{
        uint8_t valueTraversedPiece;
        uint8_t nextTraverseBitsMask;
        AG85DST1Link * linkPtr;
        TraversalStackItem(
            uint8_t setValueTraversedPiece
            , uint8_t setTraverseBitsMask
            , AG85DST1Link * setLinkPtr
            )
        : valueTraversedPiece(setValueTraversedPiece)
        , nextTraverseBitsMask(setTraverseBitsMask)
        , linkPtr(setLinkPtr)
        {            
        }
    };

    std::stack<TraversalStackItem> treeLinksPtrsStack;
    uint8_t currTraverseBitsMask = (1 << (bitsItemLength - 1));
    if (treeRoot->oneNextLinkPtr){
        treeLinksPtrsStack.emplace(
            currTraverseBitsMask
            , (currTraverseBitsMask >> 1)
            , treeRoot->oneNextLinkPtr
        );
    }
    if (treeRoot->zeroNextLinkPtr){
        treeLinksPtrsStack.emplace(
            0
            , (currTraverseBitsMask >> 1)
            , treeRoot->zeroNextLinkPtr
        );
    }
    
    size_t traversedNumbersCounter = 0;

    while(!treeLinksPtrsStack.empty()){
        
        uint8_t valueTraversedPiece = treeLinksPtrsStack.top().valueTraversedPiece;
        currTraverseBitsMask = treeLinksPtrsStack.top().nextTraverseBitsMask;
        AG85DST1Link * currTraverseLink = treeLinksPtrsStack.top().linkPtr;

        treeLinksPtrsStack.pop();

        if (currTraverseBitsMask == 0){

            resultSequence[traversedNumbersCounter] = valueTraversedPiece;
            ++traversedNumbersCounter;

        } else {

            if (currTraverseLink->oneNextLinkPtr){
                
                treeLinksPtrsStack.emplace(
                    (valueTraversedPiece | currTraverseBitsMask)
                    , currTraverseBitsMask >> 1
                    , currTraverseLink->oneNextLinkPtr
                );
            }
            if (currTraverseLink->zeroNextLinkPtr){

                treeLinksPtrsStack.emplace(
                    valueTraversedPiece
                    , currTraverseBitsMask >> 1
                    , currTraverseLink->zeroNextLinkPtr
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