#include <stdio.h>
#include <stdlib.h>

enum year_state{
    SPRING = 1,
    SUMMER,
    AUTUMN,
    WINTER
};



enum year_event{
    EVENT1 = 1,
    EVENT2,
    EVENT3,
    EVENT4,
};


typedef struct FsmTable_s{
    int event;   //事件
    int CurState;  //當前狀態
    void (*eventActFun)();  //函數指針
    int NextState;  //下一個狀態
}FsmTable_t;




void spring_thing()
{
    printf("this is spring\n");
}
void summer_thing()
{
    printf("this is summer\n");
}
void autumn_thing()
{
    printf("this is autumn\n");
}
void winter_thing()
{
    printf("this is winter\n");
}


FsmTable_t year_table[] =
{
    //{到來的事件，當前的狀態，將要要執行的函數，下一個狀態}
    { EVENT1,  SPRING,    summer_thing,  SUMMER },
    { EVENT2,  SUMMER,    autumn_thing,  AUTUMN },
    { EVENT3,  AUTUMN,    winter_thing,  WINTER },
    { EVENT4,  WINTER,    spring_thing,  SPRING },
    //add your codes here
};







typedef struct FSM_s{
    int curState;//當前狀態
    FsmTable_t * pFsmTable;//狀態表
    int size;//表的項數
}FSM_t;

/*狀態機註冊,給它一個狀態表*/
void FSM_Regist(FSM_t* pFsm, FsmTable_t* pTable)
{
    pFsm->pFsmTable = pTable;
}

/*狀態遷移*/
void FSM_StateTransfer(FSM_t* pFsm, int state)
{
    pFsm->curState = state;
}

/*事件處理*/
void FSM_EventHandle(FSM_t* pFsm, int event)
{
    FsmTable_t* pActTable = pFsm->pFsmTable;
    void (*eventActFun)() = NULL;  //函數指針初始化爲空
    int NextState;
    int CurState = pFsm->curState;
    int g_max_num = pFsm->size;
    int flag = 0; //標識是否滿足條件
    int i;

    /*獲取當前動作函數*/
    for (i = 0; i<g_max_num; i++)
    {
        //當且僅當當前狀態下來個指定的事件，我才執行它
        if (event == pActTable[i].event && CurState == pActTable[i].CurState)
        {
            flag = 1;
            eventActFun = pActTable[i].eventActFun;
            NextState = pActTable[i].NextState;
            break;
        }
    }


    if (flag) //如果滿足條件了
    {
        /*動作執行*/
        if (eventActFun)
        {
            eventActFun();
        }

        //跳轉到下一個狀態
        FSM_StateTransfer(pFsm, NextState);
    }
    else
    {
        printf("there is no match\n");
    }
}


int main()
{
    FSM_t year_fsm;
    FSM_Regist(&year_fsm,year_table);
    year_fsm.curState = SPRING;
    year_fsm.size = sizeof(year_table)/sizeof(FsmTable_t);


    printf("\n-------1--init spring------\n");
    printf("state:%d\n",year_fsm.curState);

    printf("\n-------2--spring->summer------\n");
    FSM_EventHandle(&year_fsm,EVENT1);
    printf("state:%d\n",year_fsm.curState);

    printf("\n-------3--summer->autumn------\n");
    FSM_EventHandle(&year_fsm,EVENT2);
    printf("state:%d\n",year_fsm.curState);

    printf("\n-------4--autumn->winter------\n");
    FSM_EventHandle(&year_fsm,EVENT3);
    printf("state:%d\n",year_fsm.curState);

    printf("\n-------5--winter->spring------\n");
    FSM_EventHandle(&year_fsm,EVENT4);
    printf("state:%d\n",year_fsm.curState);

    printf("\n-------6--receive EVENT2 not EVENT1------\n");
    FSM_EventHandle(&year_fsm,EVENT2);
    printf("state:%d\n",year_fsm.curState);

    return 0;
}
