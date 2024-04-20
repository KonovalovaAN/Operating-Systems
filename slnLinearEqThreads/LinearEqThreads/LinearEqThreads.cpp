// LinearEqThreads.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <process.h>


typedef double m33[3][3];
//
static m33 A = { {1,1,1}
                ,{1,2,1}
                ,{1,1,3} };
static double b[3] = { 1,0,0 };
//
static const double eps = 0.0001;
static double x[] = { 0,0,0 };
/////////////////////////////
struct matrix {
    int i;
    m33  *pA;
};
struct Query {
    matrix M;
    double Det;
    bool bRet;
};
//
bool det(matrix* pM, double& Det);
int sequentialAlgorithm();
//
unsigned __stdcall ThreadFunc(void* pArguments);
//

matrix m = { 0, (m33*)A };
double Det = 0, Det0 = 0, Det1 = 0, Det2 = 0;


int main()
{
    std::cout << "Hello World!\n";
    //return sequentialAlgorithm();
    //
    Query q = { {0,(m33*)A}, 0,false };
    Query q0 = { {1,(m33*)A}, 0,false };
    Query q1 = { {2,(m33*)A}, 0, false };
    Query q2 = { {3,(m33*)A}, 0,false };

    Query* q012[] = { &q0, &q1, &q2 };
    HANDLE hThread_q012[] = { 0,0,0 };
    unsigned threadID_q012[] = { 0,0,0 };

    HANDLE hThread_q;
    unsigned threadID_q;

    //(1)
    printf("Creating the thread for Query q...\n");
    
   // hThread_q = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, &q, 0, &threadID_q);

    hThread_q = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE) ThreadFunc, &q, 0,(LPDWORD) &threadID_q);
    if (hThread_q == INVALID_HANDLE_VALUE)
    {
        printf("the thread for Query q was not created. Last Error:%d\n", GetLastError());
        std::cout << "\nPress any Key to Quit:\n";
        std::cin.get();
        return 1;
    }

    /*
    DWORD dwretSingle = WaitForSingleObject(hThread_q, INFINITE);

    if (dwretSingle != WAIT_OBJECT_0)
    {
        printf("The function WaitForSingleObject has failed.Last Error: %d\n", GetLastError());
        std::cout << "\nPress any Key to Quit:\n";
        std::cin.get();
        return 1;
    }
    //Ok
    printf("\nAnswer to <Query q>:\n q.bRet:%s q.Det:%f\n", q.bRet?"TRUE":"FALSE", q.Det);
    */
    //(2)
    for (int i = 0; i < 3; i++)
    {
       // hThread_q012[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, q012[i], CREATE_SUSPENDED, &threadID_q012[i]);
        hThread_q012[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, q012[i], CREATE_SUSPENDED, (LPDWORD)&threadID_q012[i]);

        if (hThread_q012[i] == INVALID_HANDLE_VALUE)
        {
            printf("the thread for Query *q012[%d] was not created. Last Error:%d\n",i, GetLastError());
            std::cout << "\nPress any Key to Quit:\n";
            std::cin.get();
            return 1;
        }
    }
    //
    std::cout << "\nPress any Key to Resume 3 Threads:\n";
    std::cin.get();
    //
    for (int i = 0; i < 3; i++) ResumeThread(hThread_q012[i]);

    DWORD dwretMult=WaitForMultipleObjects(3, hThread_q012, true, INFINITE);
    if (!(dwretMult >= WAIT_OBJECT_0 && dwretMult <= WAIT_OBJECT_0+2))
    {
        printf("The function WaitForMultipleObjects has failed.Last Error: %d\n", GetLastError());
        std::cout << "\nPress any Key to Quit:\n";
        std::cin.get();
        return 1;
    }
    //Ok
    //(3)
    for (int i = 0; i < 3; i++)
    {
        printf("\nAnswer to <Query * q012[%d]>: .bRet:%s .Det:%f\n",
                                        i, q012[i]->bRet ? "TRUE" : "FALSE", q012[i]->Det);
        CloseHandle(hThread_q012[i]);
    }

    

    DWORD dwretSingle = WaitForSingleObject(hThread_q, INFINITE);

    if (dwretSingle != WAIT_OBJECT_0)
    {
        printf("The function WaitForSingleObject has failed.Last Error: %d\n", GetLastError());
        std::cout << "\nPress any Key to Quit:\n";
        std::cin.get();
        return 1;
    }
    //Ok
    printf("\nAnswer to <Query q>:\n q.bRet:%s q.Det:%f\n", q.bRet ? "TRUE" : "FALSE", q.Det);
    CloseHandle(hThread_q);
    //
    printf("\nSolving the System of Linear Equations of the Third Order:\n"); printf("\nThe equation of Ax=b:\n");
    for (int i = 0; i < 3; i++)
    {
        printf("\n||");
        for (int j = 0; j < 3; j++)
            printf("%10.4f ", A[i][j]);
        printf("||%4s[%2d] = %10.4f", "x", i, b[i]);

    }
    printf("\n");
    for (int i = 0; i < 3; i++)
        if (q012[i]->bRet)
        {
            x[i] = q012[i]->Det / q.Det;
            printf("x[%d] = %f\n",i, x[i]);
        }
        else
            printf("x[%d] = ? because q012[%d]->bRet = %s \n",i,i, q012[i]->bRet ? "TRUE" : "FALSE");

    std::cout << "\nPress any Key to Quit:\n";
    std::cin.get();
    return 0;

}
unsigned __stdcall ThreadFunc(void* pArguments)
{
    //Calculate the detM for any m33 M, M in {A,A0,A1,A3)
    Query* qr = (Query*)pArguments;
    matrix* pM= &(qr->M);
    double Det = (qr->Det);

    qr->bRet = det(&(qr->M), qr->Det);    

   // _endthreadex(0); needed if _beginthreadex is used
    return 0;
}

 int sequentialAlgorithm()
{

    bool bRet = det(&m, Det);

    printf("\nDet:%f\n", Det);
    if (!bRet)
    {
        printf("abs(Det)<= %f\n", eps);
        return 1;
    }
    //x0
    m.i = 1;
    bRet = det(&m, Det0);


    printf("\nDet0:%f Det:%f\n", Det0, Det);
    printf("x0:%f\n", Det0 / Det);
    //x1
    m.i = 2;
    bRet = det(&m, Det1);

    printf("\nDet1:%f Det:%f\n", Det1, Det);
    printf("x1:%f\n", Det1 / Det);
    //x2
    m.i = 3;
    bRet = det(&m, Det2);

    printf("\nDet2:%f Det:%f\n", Det2, Det);
    printf("x2:%f\n", Det2 / Det);


    std::cout << "\nPress any Key to Quit:\n";
    std::cin.get();
    return 0;

}

bool det(matrix * pM, double &Det)
{
    bool bRes = false;
     Det = 0;
    if(pM->i<0 && pM->i>3)
        return false;
    
    double a00 = (*(pM->pA))[0][0],  a01 = (*(pM->pA))[0][1], a02= (*(pM->pA))[0][2];//a02 = (*(pM->pA))[0][2];   
    //
    double a10 = (*((*pM).pA))[1][0], a11 = (*((*pM).pA))[1][1], a12 = (*(pM->pA))[1][2];//a12 = (*(pM->pA))[1][2]; //a12 = (*(*pM).pA)[1][2]; //Er: a12 = *(*pM).pA[1][2]; //Er: a12 = *((*pM).pA)[1][2];
    //
    double a20 = (*(pM->pA))[2][0],  a21 = (*(pM->pA))[2][1],    a22 = (*(pM->pA))[2][2];

    switch (pM->i)
    {
        case 0:        
            break;
        case 1:
            a00 = b[0]; a10 = b[1]; a20 = b[2];
            break;
        case 2:
            a01 = b[0]; a11 = b[1]; a21 = b[2];
            break;
        case 3:
            a02 = b[0]; a12 = b[1]; a22 = b[2];
            break;
    default:
        return bRes;
        break;
    }

    Det = a00 * (a11 * a22 - a12 * a21)
        - a01 * (a10 * a22 - a12 * a20)
        + a02 * (a10 * a21 - a11 * a20);

    bRes = (abs(Det) < eps) && pM->i==0 ? false : true;
    return bRes;
    
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
