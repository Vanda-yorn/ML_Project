
// A structure to represent an adjacency list node
struct node{
    int data;
    char edgetype;
    struct node* next;
}typedef node;

// Adds an edge to an adjacency list
node* push(node* first , char edgetype , int data)
{
    if (first==NULL){
        node* new_node = (node*)malloc(sizeof(node));
        new_node->edgetype = edgetype;
        new_node->data = data;
        new_node->next = NULL;
        first = new_node;
        return new_node;
    }
    first->next = push(first->next,edgetype,data);
    return first;
}

struct element{                 //for queue
    int data;
    struct element * next;
}typedef element;

struct list{
    int n;
    element* front;
    element* rear;
}typedef list;

list* createQueue(){
    list*ls;
    ls= (list*)malloc(sizeof(list));

    ls->front =NULL;
    ls->rear =NULL;
    ls->n=0;

    return ls;
}
element* newElement(int k)           //create an element with data"k"
{
    struct element* temp = (struct element*)malloc(sizeof(struct element));
    temp->data = k;
    temp->next = NULL;
    return temp;
}

//add to end of queue
void enqueue(list* ls ,int value){
    element*e;
    e= newElement(value);

    if(ls->n ==0){
        ls->front =e;
        ls->rear =e;
    }
    else{
        ls->rear->next = e;
        ls->rear = e;
    }

    ls->n += 1;
}

void dequeue(list*ls){
    if(ls->n==0){
        printf("\n\tQueue Underflow");
    }
    else{
        element*tmp;
        tmp=ls->front;

        ls->front =tmp->next;
        free(tmp);
        ls->n -=1;
    }

}
void displayQueue(list*ls){
    if(ls->n==0){
        printf("\n\tQueue is empty.");
    }
    else{
        element*tmp;
        tmp=ls->front;


            while(tmp != NULL){
            printf("%d \n",tmp->data);
            tmp=tmp->next;
            }
    }
}
list* deletequeue(list*q)
{
    struct element* temp;
    temp=q->front;

    while(temp != NULL)
    {
        temp = temp->next;
        q->n --;
        free(q->front);
        q->front=temp;
    }
    return q;
}


 //fucntion for copy queue
list * copyQ(list *q) {

    element *current = q->front;  //holds the current node

    list *copy = createQueue();

    //traverses the list
    while (current != NULL) {
        enqueue(copy,current->data);
        current = current->next;
    }
    return copy;
}





int checkfinalstate(list* queue,int* final,int nf){
    element* temp=queue->front;

    for(int i=0;i<queue->n;i++){
        for(int j=0;j<nf;j++){
            if(temp->data==final[j]){
                return 1;
                break;
            }
        }
        temp=temp->next;
    }
    return 0;
}
void displayTable(node** graph,int n ,char* symbol,int n_symbol,int* final, int nf){
    printf("\n");
    printf("\t\t\t\t\t|");
    for(int i=0; i<n_symbol+1; i++){
        printf("\t%c\t|",symbol[i]);
    }
    printf("\n\t\t\t\t========");
    for(int i=0; i<n_symbol+1; i++){
        printf("================");
    }
    printf("\n");
    for(int i=0;i<n;i++){
        printf("\t\t\t\t  ");
        for(int k=0; k<nf; k++){
            if(final[k]==i) printf("*");
        }
        if(i==0) printf("->");
        printf("q%d\t|\t",i);
        for(int j=0; j<n_symbol+1; j++){
            node* temp = graph[i];
            node* temp1 = graph[i];

            int count=0;
            int count1=0;

            while(temp!=NULL){
                if(temp->edgetype==symbol[j]){
                    printf("q%d ",temp->data);
                    if(temp->edgetype=='e') count1++;
                }else if(temp->edgetype!='e'&&j==n_symbol){
                    count1++;
                    if(count1==1) printf("-");
                    else         printf("");
                }
                temp=temp->next;
            }
            printf("\t|\t");
        }
        printf("\n\t\t\t\t========");
        for(int i=0; i<n_symbol+1; i++){
            printf("================");
        }
        printf("\n");
    }
}
//function to test string:
int teststring( node** graph, list*queue ,char * input,int* final, int nf ,int index){

    if (index==(int)strlen(input)){
        return checkfinalstate(queue,final,nf);
    }

    int k=queue->n;
    for(int i=0;i<k;i++){
        element* temp1 =queue->front;           //temp1 for front of the queue
        node* temp = graph[temp1->data];        //temp for adjacency list of graph
        while (temp != NULL){
            if (input[index] == temp->edgetype){
                enqueue(queue,temp->data);

            }
            temp=temp->next;
        }
        dequeue(queue);

    }
    teststring(graph, queue ,input,final ,nf,index+1);
}


//function to test DFA or NFA
int checkDfaNfa(node** graph,int n,char* symbol, int n_symbol){

    for(int i=0;i<n;i++){
        int count=0;

        node* temp = graph[i];
        while(temp!=NULL){
            temp=temp->next;
            count++;
        }


        if(count != n_symbol){
            return 0;
            break;
        }

        char c[n_symbol];
        int j=0;
        node* temp1 = graph[i];

        if(count==n_symbol){
            while(temp1!=NULL){
                c[j]=temp1->edgetype;
                temp1=temp1->next;
                j++;
            }
        }



        for(int i=0;i<n_symbol;i++){
            for(int k=i+1;k<n_symbol;k++){
                if(c[i]==c[k]){
                    return 0;
                    break;
                }
            }
        }

    }

    return 1;
}

bool checkIfExistQ(list* Q,int data){           //check if data is already in Q
    element* temp=Q->front;
    for(int i=0;i<Q->n;i++){
        if( temp->data==data ){
            return true;
            break;
        }
        temp=temp->next;
    }
    return false;
}

int checkIfExistSqSum(int * sqsum,int p, int data){
    for(int i=0;i<p;i++){
        if(sqsum[i]==data){
            return i;
            break;
        }
    }
    return -1;          //not exist
}


void    convertNfaDfa(node** graph,int* final, int nf ,char* symbol,int n_symbol,int c){

    node* g[20];
    list * Q[20];
    int sq_sum[20];

    for(int i=0;i<20;i++){
        g[i]=NULL;
    }

    list* Qtemp,*Qtemp1;
    int newfinal[20],newnf=0;

    Q[0]=createQueue();  //initialize 0 as new state
    enqueue(Q[0],0);
    Qtemp1=createQueue();

    ////check if state 0 have epsilon transition

    node* temp = graph[0];         //temp for adjacency list of graph
    while ( temp != NULL){
        if ( temp->edgetype == 'e'){
            if(!checkIfExistQ(Q[0],temp->data)){         //check if state by epsilon closure already exist in Q
                enqueue(Q[0],temp->data);
            }
        }
        temp=temp->next;
    }

    int s=0;
    element* Temp = Q[0]->front;
    while(Temp!=NULL){
        //element* temp =Qtemp->front;
        s += pow(2,Temp->data) ;
        Temp=Temp->next;
    }

    sq_sum[0]=s;    //start state
    int t=0,p=1;            //current number of element in sq_sum

    int z=checkfinalstate(Q[0],final,nf);
    if(z==1){
        newfinal[newnf]=0;
        newnf++;
    }



    while( t < p){
        for(int j=0;j<n_symbol;j++){
            Qtemp=copyQ(Q[t]);
            Qtemp1=deletequeue(Qtemp1);

            // transition through symbol
            int k=Qtemp->n;
            for(int i=0;i<k;i++){
                element* temp1 =Qtemp->front;           //temp1 for front of the queue
                node* temp = graph[temp1->data];        //temp for adjacency list of graph
                while (temp != NULL){
                    if ( temp->edgetype == symbol[j]){
                        if(!checkIfExistQ(Qtemp1,temp->data)){       //////////////////////////
                           enqueue(Qtemp1,temp->data);
                        }
                    }
                    temp=temp->next;
                }
                dequeue(Qtemp);
            }

            Qtemp=copyQ(Qtemp1);

            // epsilon-closure
            k=Qtemp->n;                                  //new number of node in queue
            for(int i=0;i<k;i++){
                element* temp1 =Qtemp->front;            //temp1 for front of the queue
                node* temp = graph[temp1->data];        //temp for adjacency list of graph
                while (temp != NULL){
                    if ( temp->edgetype == 'e'){
                        if(!checkIfExistQ(Qtemp1,temp->data)){         //check if state by epsilon closure already exist in Q
                           enqueue(Qtemp1,temp->data);
                        }
                    }
                    temp=temp->next;
                }
                dequeue(Qtemp);
            }

            int s=0;
            element* temp = Qtemp1->front;
            while(temp!=NULL){
                s += pow(2,temp->data) ;
                temp=temp->next;
            }

            int index=checkIfExistSqSum(sq_sum,p,s);
            if(index==-1){              //new state is found
                sq_sum[p]=s;
                Q[p]=copyQ(Qtemp1);
                g[t]=push(g[t],symbol[j],p);

                // find new final state;
                int c=checkfinalstate(Qtemp1,final,nf);
                if(c==1){       //Qtemp1 contain final state
                   newfinal[newnf]=p;
                   newnf++;
                }

                p++;
            }
            else{
                g[t]=push(g[t],symbol[j],index);
            }
        }
        t++;
    }

    if(c==0){
        printf("\n\t\t\t\tTransition of the equivalent DFA:\n");

        displayTable(g,p,symbol,n_symbol,newfinal,newnf);
    }

    if(c==1){
        minimizeDFA(g,p,newfinal,newnf,symbol,n_symbol);
    }

}

bool checkIfMarked(int a , int b, int k,int mat[k][k]){
    if(a>=b){
        if( mat[a][b]==1 ){
            return true;
        }
        else if(mat[a][b]==0){
            return false;
        }
    }
    else if(a<b){
        if( mat[b][a]==1 ){
            return true;
        }
        else if(mat[b][a]==0){
            return false;
        }
    }
}

bool matrixChanged( int k, int mat1[k][k], int mat2[k][k] ){
    for(int i=0;i<k-1;i++){
        for(int j=i+1;j<k;j++){
            if( mat1[j][i] != mat2[j][i]){
                return true;
                break;
            }
        }
    }
    return false;
}


//function for minimizationminimizeDFA
void minimizeDFA(node** graphOriginal,int n_state,int* final, int nf ,char* symbol,int n_symbol){
    ////copy graph
    node* graph[n_state];
    for(int i=0;i<n_state;i++){
        graph[i]=graphOriginal[i];
    }

    list *queue = createQueue();
    list* queue1=createQueue();
    enqueue(queue,0);
    enqueue(queue1,0);



    ////find all accessible state:
    while(queue1->n!=0){
        element* temp1 = queue1->front;
        node* temp = graph[temp1->data];//temp for adjacency list of graph
        while (temp != NULL){
            if(!checkIfExistQ(queue,temp->data)){       //////////////////////////
                    enqueue(queue,temp->data);
                    enqueue(queue1,temp->data);
            }
            temp=temp->next;
        }
        dequeue(queue1);
    }

    //check non accessible state


    int as[n_state];       //array for store accessible states
    int nas[n_state];    //array for store non accessible states
    int n_nas=0;            //number of non accessible state
    int n_as=0;             //number of accessible state

    for(int i=0; i<n_state; i++){
        if(!checkIfExistQ(queue,i)){
            nas[n_nas] = i;
            n_nas++;
        }
        else{
            as[n_as]=i;
            n_as++;
        }
    }

    //delete non accessible state
    for (int i=0;i<n_nas;i++){          //delete graph of non accessible state
        graph[nas[i]]=NULL;
    }

    int mat[n_as][n_as];
    int matcopy[n_as][n_as];
    //initail zero to lower triangle of matrix
    for(int i=0;i<n_as;i++){                //i : column    j:row
        for(int j=0;j<n_as;j++){
            mat[j][i]=0;
            matcopy[j][i]=0;
        }
    }

    //first iteration, mark all pairs that contain final state, but not include pair of 2 final states
    for(int i=0;i<n_as-1;i++){
        for(int j=i+1;j<n_as;j++){
            for(int k=0;k<nf;k++){
                if( as[i]==final[k]){
                    mat[j][i]=1;
                }
            }
        }
    }
    for(int i=0;i<n_as-1;i++){
        for(int j=i+1;j<n_as;j++){
            for(int k=0;k<nf;k++){
                if( as[j]==final[k]){
                    if(mat[j][i]==0){
                       mat[j][i]=1;
                    }
                    else if(mat[j][i]==1){  //exclude pair of 2 final states;
                       mat[j][i]=0;
                    }
                }
            }
        }
    }

    while ( matrixChanged(n_as, mat ,matcopy) ){
     //copy matrix to compare

        for(int i=0;i<n_as-1;i++){
            for(int j=i+1;j<n_as;j++){
                matcopy[j][i]=mat[j][i];
            }
        }

        int x1,x2,index_x1,index_x2;
        for(int i=0;i<n_as-1;i++){
            for(int j=i+1;j<n_as;j++){
                if(mat[j][i]==0){           //for pair that is not marked yet
                    for(int t=0;t<n_symbol;t++){            //note that: each pair of state is ( as[i],as[j]  )  i,j index of matrix
                        node* temp = graph[as[i]];          //find as[i] transition to what?
                        while (temp != NULL){
                            if ( symbol[t] == temp->edgetype){
                                x1=temp->data;
                                break;
                            }
                            temp=temp->next;
                        }
                        node* temp1 = graph[as[j]];        //find as[j] transition to what?
                        while (temp1 != NULL){
                            if ( symbol[t] == temp1->edgetype){
                                x2=temp1->data;
                                break;
                            }
                            temp1=temp1->next;
                        }
                        //get index
                        for(int r=0;r<n_as;r++){
                            if(as[r]==x1){
                                index_x1=r;
                                break;
                            }
                        }
                        for(int r=0;r<n_as;r++){
                            if(as[r]==x2){
                                index_x2=r;
                                break;
                            }
                        }

                        //check if the pair of after-transition of ( as[i],as[j] ) is already marked?
                        if( checkIfMarked( index_x1,index_x2 ,n_as, mat ) ){
                            mat[j][i]=1;
                            break;
                        }

                    }

                }

            }
        }
    }

    int deletestate[n_as];
    int n_delstate=0;
    for(int i=0;i<n_as-1;i++){
        for(int j=i+1;j<n_as;j++){
            if(mat[j][i]==0){
                deletestate[n_delstate]=as[j];
                n_delstate++;

                //delete one of the equivalent state : delete its graph
                // here we delete big-number state which is as[j]
                graph[as[j]]=NULL;

                // change all as[j] to as[i]
                for(int k=0;k<n_as;k++){
                        node* temp = graph[as[k]];
                        while (temp != NULL){
                            if ( as[j] == temp->data){
                                temp->data=as[i];
                            }
                            temp=temp->next;
                        }
                }

            }
        }
    }

    int n_newstate=0; //number of accessible state - number of deleted state
    int newstate[n_as];
    int n_newfinal=0;
    int newfinal[nf];

    //new state:
    for(int k=0;k<n_as;k++){
        int r=0;
        for(int j=0;j<n_delstate;j++){
            if( as[k] == deletestate[j] ){
                r=1;        //meaning it is the deleted state
                break;
            }

        }
        if(r==0){
            newstate[n_newstate]=as[k];
            n_newstate++;
        }
    }

    //find new final state after delete equivalent state;
    for(int k=0;k<nf;k++){
        int re=0;
        for(int j=0;j<n_delstate;j++){
            if( final[k] == deletestate[j] ){
                re=1;        //meaning it is the deleted state
                break;
            }
        }
        if(re==0){
            newfinal[n_newfinal]=final[k];
            n_newfinal++;
        }
    }

    //change to new states to order index (0,1,2,3,....)
    for(int i=0;i<n_as;i++){
            node* temp = graph[i];
            while (temp != NULL){
                for(int k=0;k<n_newstate;k++){
                    if ( newstate[k] == temp->data){
                        temp->data= k;
                        break;
                    }
                }
                temp=temp->next;
            }
    }


    ////change index of new final state
    for (int i=0;i<n_newfinal;i++){
        for(int j=0;j<n_newstate;j++){
            if(newfinal[i]==newstate[j]){
                newfinal[i]=j;
                break;
            }
        }
    }


    ////

    node* newgraph[n_newstate];
    for(int k=0;k<n_newstate;k++){
        newgraph[k]=graph[newstate[k]];
    }



    printf("\n\n\t\t\t\tTransition of the minimize DFA:\n");
    displayTable(newgraph,n_newstate,symbol,n_symbol,newfinal,n_newfinal);





}



