#include<stdio.h>
#include<time.h>
#include<string.h>
#include<sqlite3.h>
#include<stdbool.h>
#include <Windows.h>
#include "function.h"
#include "design.h"
#include "database.h"
sqlite3 *db;
char *zErrMsg = 0;
int rc;

int id;
char symbols[10][2];
int state[50];
int start_state[50];
int final_state[50];
int state_tran[50];
char symbol_tran[50][2];
int next_states[50];
char symbol1[10],state1[20], start[20],final[20],next_state1[10];

void cls();
void insertData();
void getFromDatabase();
void getID(void *data,int argc, char **argv, char **azColName);
void InsertToFA(char fa_type[10],char description[50]);
void InsertToAp(int id,char symbol);
void InsertToST(int id,int state,int start_state,int final_state);
void InsertToTrans(int id,int state,char symbol,int next_state);

int loadFa(void *data, int argc, char **argv, char **azColName) {// display all FA in database

    printf("\n\t\t\t\t\t[%s]  %s\n\n", argv[0],argv[2]);
    return 0;
}

int Alp=0;
int loadAlp(void *data, int argc, char **argv, char **azColName) {       // get data from table Alphabet
    sprintf(symbols[Alp],"%s",argv[1]);
    Alp++;
    return 0;
}

int st=0;
int loadState(void *data, int argc, char **argv, char **azColName) {       // get data from table states
    state[st] = atoi(argv[1]);
    start_state[st] = atoi(argv[2]);
    final_state[st] = atoi(argv[3]);
    st++;
    return 0;

}

int tr=0;
int loadTran(void *data, int argc, char **argv, char **azColName) {       // get data from table transition
    state_tran[tr]= atoi(argv[1]);
    sprintf(symbol_tran[tr],"%s",argv[2]);
    next_states[tr]= atoi(argv[3]);
    tr++;
    return 0;
}


int main(){
    system("Color F0");

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD size;

    if (GetFontSize(h, &size)){
        /* Grow by 50% */
        size.X += (SHORT)(size.X * .5);
        size.Y += (SHORT)(size.Y * .5);
        SetFontSize(h, size);
    }

    Ascii_Art();
    loading();

    rc = sqlite3_open("Automata.db", &db);
    createDataBase(); //function for create database
    char opt;
    while(1){
        cls();

        char mainmenu[3][50]={"[1]   Load FA From Database","[2]   Create FA","[3]   Exit"};
        char head1[50]="  MAIN MENU";
        int m1=sizeof(mainmenu)/sizeof(mainmenu[0]);
        int num=choice(mainmenu,head1,m1);

        ///////reset global var.
        memset(symbols,0,10);
        memset(state,0,50);
        memset(start_state,0,50);
        memset(final_state,0,50);
        memset(state_tran,0,50);
        memset(symbol_tran,0,50);
        memset(next_states,0,50);
        /////////



        if(num==1) {

                cls();

                int fa_id;
                char *sql;
                char sql1[200],sql2[200],sql3[200];
                header("FA FILE");
                sql = "SELECT * FROM FA;";
                sqlite3_exec(db, sql, loadFa, 0, &zErrMsg);

                printf("\n\t\t\t\t\t\tEnter File ID: ");SetColor(0);
                scanf("%d",&fa_id);SetColor(9);

                sprintf(sql1,"SELECT * FROM ALPHABET WHERE fa_id=%d;",fa_id);
                sqlite3_exec(db, sql1, loadAlp, 0, &zErrMsg);

                sprintf(sql2,"SELECT * FROM STATES WHERE fa_id=%d;",fa_id);
                sqlite3_exec(db, sql2, loadState, 0, &zErrMsg);

                sprintf(sql3,"SELECT * FROM transition WHERE fa_id=%d;",fa_id);
                sqlite3_exec(db, sql3, loadTran, 0, &zErrMsg);

                int n,nf=0;
                int i,j,n_symbol;

                n_symbol = Alp;

                char symbol[n_symbol+1];

                for(i=0;i<n_symbol;i++){

                    symbol[i] = symbols[i][0];
                }
                symbol[n_symbol]='e';           // epsilon

                n = st;
                node* graph[n]; //Create a graph
                int finals[nf];    //Array to store state of vertex

                for(int i=0; i<n; i++){
                    if(final_state[i]==1){
                        finals[nf] = i;
                        nf++;
                    }
                }


                int finalcpy[nf];
                for(int i=0;i<nf;i++){
                    finalcpy[i]=finals[i];
                }


                for (i=0;i<n;i++){            //create empty adjacency list
                    graph[i]=NULL;
                }

                for (i=0; i<tr; i++){
                    graph[state_tran[i]] = push(graph[state_tran[i]],symbol_tran[i][0],next_states[i]);
                }
                Alp=0;
                st=0;
                tr=0;

                /////menu 5 function
                while(1){
                    cls();

                    char submenu[5][50]={"[1]   Display Transition Table","[2]   Test String","[3]   Convert NFA to DFA","[4]   Minimize FA","[5]   Back" };
                    char head1[50]="   MENU";
                    int m2=sizeof(submenu)/sizeof(submenu[0]);
                    int num1=choice(submenu,head1,m2);

                    if(num1==5){

                    //// delete graph
                        for(int i=0;i<n;i++){
                            graph[i]=NULL;

                        }
                        break;
                    }
                    switch(num1) {
                        case 1:
                            cls();
                            header("Transitions Table");
                            displayTable(graph,n ,symbol,n_symbol,finalcpy,nf);
                            printf("\n\t\t\t\t");
                            system("pause");
                            break;
                        case 2:
                            cls();
                            header("Test String");
                            while(1){
                                int result;
                                int input[50];
                                printf("\n\n\t\t\t\t\t[input 0 to go back]");
                                printf("\n\t\t\t\t\tinput a string:");SetColor(0);
                                scanf("%s",&input);SetColor(9);

                                if(strcmp(input,"0")!=0){
                                    list*queue=createQueue();
                                    enqueue(queue,0);          //start state is 0

                                    node* temp=graph[0];
                                    while(temp != NULL){
                                        if(temp->edgetype=='e'){
                                            enqueue(queue,temp->data);
                                        }
                                        temp=temp->next;
                                    }

                                    result=teststring(graph,queue,input,finalcpy,nf,0);
                                    if(result==1){
                                        printf("\n\t\t\t\t\t=> String is");
                                        SetColor(10);
                                        printf(" accepted");SetColor(9);
                                    }
                                    else{
                                        printf("\n\t\t\t\t\t=> String is ");
                                        SetColor(4);
                                        printf("not accepted");SetColor(9);
                                    }
                                }else   break;
                            }
                            break;
                        case 3:
                            cls();
                            header("Convert NFA to DFA");
                            if(checkDfaNfa(graph,n,symbol,n_symbol)==0){ //NFA return 0
                                printf("\n\n\t\t\t\t\tIt is a NFA!\n");
                                convertNfaDfa(graph,finalcpy,nf,symbol,n_symbol,0);
                            }else{
                                printf("\n\n\t\t\t\t\tIt is already a DFA!");
                            }
                            printf("\n\n\t\t\t\t");
                            system("pause");
                            break;
                        case 4:
                            cls();
                            header("Minimize DFA");
                            convertNfaDfa(graph,finalcpy,nf,symbol,n_symbol,1);
                            printf("\n\n\n\t\t\t\t");
                            system("pause");
                            break;
                       default :
                           break;

                    }

                }

                cls();

        }else if(num==2){

            cls();
            insertData();
            cls();
        }else if(num==3){
                break;
        }
    }
    sqlite3_close(db);
}

void cls(){
	system("cls");
}

void insertData(){
    int n,nf;
    int i,j,n_symbol;
    header("CREATE FA");
    char fa_type[10],description[50];
    printf("\n\n\t\t\t\t\tInput FA tpye: "); scanf("%s",&fa_type );
    printf("\n\t\t\t\t\tDescription: "); scanf(" %[^\t\n]s",&description);


    printf("\n\n\t\t\t\t\tInput the number of symbol:");
    scanf("%d",&n_symbol);
    char symbol[n_symbol+1];

    for(i=0;i<n_symbol;i++){
        printf("\t\t\t\t\t\tsymbol#%d: ",i+1);
        scanf("%c",&symbol[i]);
        scanf("%c",&symbol[i]);

    }
    symbol[n_symbol]='e';           // epsilon


    //Number of state and final state
    printf("\n\t\t\t\t\t> number of state:");
    scanf("%d", &n);
    printf("\n\t\t\t\t\t> number of final state:");
    scanf("%d",&nf);

    node* graph[n+1]; //Create a graph
    int final[nf]; //Array to store state of vertex

    for (i=0;i<nf;i++){
        if(nf==1){
            printf("\n\t\t\t\t\t\tPosition of final state: ");
        }else{
            printf("\n\t\t\t\t\t\tPosition of final state#%d : ",i+1);
        }

        scanf("%d",&final[i]);
    }







    for (i=0;i<n+1;i++){            //create empty adjacency list
        graph[i]=NULL;
    }

    int tran[50];
    char sym_tran[50];
    int next_state2[50];

    int s=0;
    for (i=0; i<n; i++){
        printf("\n\n\t\t\t\t\t\t|> state q%d <|\n",i);

        int num_trans;             //Index of vertex , Number of edges

        printf("\n\t\t\t\t\tnumber of transitions:");
        scanf("%d",&num_trans);


        //Add all edges
        for (j=0;j<num_trans;j++) {
            int next_state3;
            char sym;
            printf("\n\t\t\t\t\t\tTransition %d:\n\t",j+1);
            printf("\t\t\t\t\t\tSymbol:");
            scanf("%c",&sym);
            scanf("%c",&sym);
            printf("\t\t\t\t\t\t\tTo State:");
            scanf("%d",&next_state3);

            tran[s] = i;
            sym_tran[s] = sym;
            next_state2[s] = next_state3;
            //InsertToTrans(id,i,sym,next_state3);
            graph[i] = push(graph[i],sym,next_state3);
            s++;
        }
    }

    //store to Fa table
    InsertToFA(fa_type,description);

    for(i=0;i<n_symbol;i++){
        InsertToAp(id,symbol[i]);
    }


    //store state,startstate, finalstate into database
    for (i=0; i<n; i++){
            int r=0;
            if ( i==0 ){
                for (int j=0;j<nf;j++){
                    if (i==final[j]){
                        r=1;
                        break;
                    }

                }
                InsertToST(id,i,1,r);
            }

            else{
                for (int j=0;j<nf;j++){
                    if (i==final[j]){
                        r=1;
                        break;
                    }
                }
                InsertToST(id,i,0,r);
            }

        }

    for(int i=0; i<s; i++){
        InsertToTrans(id,tran[i],sym_tran[i],next_state2[i]);
    }
/////menu 5 function
    while(1){
        cls();

        char submenu[5][50]={"[1]   Display Transition Table","[2]   Test String","[3]   Convert NFA to DFA","[4]   Minimize FA","[5]   Back" };
        char head1[50]="   MENU";
        int m2=sizeof(submenu)/sizeof(submenu[0]);
        int num1=choice(submenu,head1,m2);
        if(num1==5){
            break;
        }
        switch(num1) {
            case 1:
                cls();
                header("Transition Table");
                displayTable(graph,n ,symbol,n_symbol,final,nf);
                printf("\n\t\t\t\t");
                system("pause");
                break;
            case 2:
                cls();
                header("Test String");
                while(1){
                    int result;
                    int input[50];
                    printf("\n\n\t\t\t\t\t[input 0 to go back]");
                    printf("\n\t\t\t\t\tinput a string:");scanf("%s",&input);

                    if(strcmp(input,"0")!=0){
                        list*queue=createQueue();
                        enqueue(queue,0);          //start state is 0

                        ////
                        node* temp=graph[0];
                        while(temp != NULL){
                            if(temp->edgetype=='e'){
                                enqueue(queue,temp->data);
                            }
                            temp=temp->next;
                        }

                        result=teststring(graph,queue,input,final,nf,0);
                        if(result==1){
                            printf("\n\t\t\t\t\t=> String is");
                            SetColor(10);
                            printf(" accepted");SetColor(9);
                        }
                        else{
                            printf("\n\t\t\t\t\t=> String is ");
                            SetColor(4);
                            printf("not accepted");SetColor(9);
                        }
                    }else   break;
                }
                break;
            case 3:
                cls();
                header("Convert NFA to DFA");
                if(checkDfaNfa(graph,n,symbol,n_symbol)==0){ //NFA return 0
                    printf("\n\n\t\t\t\t\tIt is a NFA!\n");
                    convertNfaDfa(graph,final,nf,symbol,n_symbol,0);
                }else{
                    printf("\n\n\t\t\t\t\tIt is already a DFA!");
                }
                printf("\n\n\t\t\t\t");
                system("pause");
                break;
            case 4:
                cls();
                header("Minimize DFA");
                convertNfaDfa(graph,final,nf,symbol,n_symbol,1);
                printf("\n\n\n\t\t\t\t");
                system("pause");
                break;
            default :
                break;

        }

    }

    cls();

}

void getID(void *data,int argc, char **argv, char **azColName){
        id=atoi(argv[0])+1; // take max old id +1 to get next id
}
void InsertToFA(char fa_type[10],char description[50]){
    char sqlInsertFA[200];
    ////query to find next id
    ////
    char sqlFindId[40];

    //sqlFindId="SELECT MAX(id) FROM FA;";
    sprintf(sqlFindId, "SELECT MAX(id) FROM FA");
    rc = sqlite3_exec(db, sqlFindId, getID, 0, &zErrMsg);

    sprintf(sqlInsertFA, "INSERT INTO FA VALUES(%d, '%s', '%s');", id, fa_type, description);
    rc = sqlite3_exec(db, sqlInsertFA, NULL, 0, &zErrMsg);

}
void InsertToAp(int id,char symbol){
    char sqlInsertAP[200];

    sprintf(sqlInsertAP, "INSERT INTO  alphabet VALUES(%d, '%c');", id, symbol);

   // printf("%s",sqlInsertAP);
    rc = sqlite3_exec(db, sqlInsertAP, NULL, 0, &zErrMsg);
}

void InsertToST(int id,int state,int start_state,int final_state){
    char sqlInsertST[200];

    sprintf(sqlInsertST, "INSERT INTO states VALUES(%d, %d, %d,%d);", id, state,start_state, final_state);
    rc = sqlite3_exec(db, sqlInsertST, NULL, 0, &zErrMsg);
}

void InsertToTrans(int id,int state,char symbol,int next_state){
    char sqlInsertTran[200];

    sprintf(sqlInsertTran, "INSERT INTO transition VALUES(%d, %d, '%c', %d);", id, state,symbol,next_state );
    rc = sqlite3_exec(db, sqlInsertTran, NULL, 0, &zErrMsg);
}

/*
dfa
blahbbbbaa
2
a
b
2
1
1
2
a
0
b
1
2
a
0
b
1


*/
