.. title:: clang-tidy - gjb8114-unsigned-compare-with-signed

gjb8114-unsigned-compare-with-signed
====================================

Direct comparison between unsigned and signed numbers is prohibited.

An example:

.. code-block:: c++

    int TestCaseUsignedCompareWithSigned(void){
        unsigned int x;
        int y ,i; 
        x=2;
        y=-2;
        if(y<x) //违背1
        {
            i=0;
        }
        else
        {
            i=1;
        }
        return(0);
    }

遵循示例:

.. code-block:: c++

    int main(void){
        unsigned int x;
        int y ,i; 
        x=2;
        y=-2;
        if(y < (int)x) //遵循1
        {
            i=0;
        }
        else
        {
            i=1;
        }
        return(0);
    }
