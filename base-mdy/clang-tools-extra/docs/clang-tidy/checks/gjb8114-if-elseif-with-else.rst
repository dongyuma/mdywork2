.. title:: clang-tidy - gjb8114-if-elseif-with-else

gjb8114-if-elseif-with-else
===========================

CSR-161: 在if-else if语句中必须使用else分支

GJB 8114-2013:准则R-1-4-1 在if-else if语句中必须使用else分支

违背示例:

.. code-block:: c++

  int main_GJB8114_2013_1_4_1(void)
    {
        int i = 0, j =0;
        double x = 0.0;

        if (0 == i)
        {
            x = 1.0;
        }
        else if (1 == i)
        {
            x = 2.0;
        } //违背1
        if ( 0 == j)
        {
            x = x + 5.0;
        }
        return (0);
    }

遵循示例�

.. code-block:: c++

    int main_GJB8114_2013_1_4_1_f(void)
    {
        int i = 0, j =0;
        double x = 0.0;

        if (0 == i)
        {
            x = 1.0;
        }
        else if (1 == i)
        {
            x = 2.0;
        }
        else //遵循1
        {
            x = 0.0;
        }

        if ( 0 == j)
        {
            x = x + 5.0;
        }
        return (0);
    }