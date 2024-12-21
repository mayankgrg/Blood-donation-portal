#include <iostream>
#include<bits/stdc++.h>
#include<string>
#include<fstream>
//#include<windows.h>
#include<graphics.h>
#include<stdlib.h>
using namespace std;

COORD coord={0,0};

void gotoxy(int x,int y)
{
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

int c=0;
void img()
{
    if(c==0)
    {
        initwindow(885,500,"MAP");
        readimagefile("mini_mini_map.jpg",0,0,885,500);
    }
    c++;
}

class Patient
{
    string name;
    int age;

public:
    string bloodType;
    string location;
    void getPatient()
    //this function will take details and save it to a file
    // for further future use(for notifying when available).
    {
        img();
        gotoxy(8,3);
        cout<<"ENTER THE DETAILS OF PATIENT AND LOCATION";

        gotoxy(10,7);
        cout<<"Blood Type\t\t\t: ";
        gotoxy(10,8);
        cout<<"Enter location\t\t: ";

        gotoxy(45,7);
        fflush(stdin);
        getline(cin, bloodType);
        gotoxy(45,8);
        fflush(stdin);
        getline(cin,location);
        transform(location.begin(), location.end(), location.begin(), ::toupper);
    }
};

//this function is used to store the list of location in an array to make easier to find in next steps.
void outVertex(ifstream& vertex, string locations[])
{
    int i=0;
    string loc;
    while(!vertex.eof())
    {
        while( getline(vertex, loc))
           {
               i++;
               locations[i]=loc;
           }
    }

}
void DetailsToFile(ofstream& file, string LOCATION, string NAME, string BTYPE, string ORGANISATION, string PHONE)
{
    gotoxy(5,3);
    cout<<"SUBMIT DETAILS OF DONORS: ";
    gotoxy(8,5);
    cout<<"Location \t\t: ";
    fflush(stdin);


    gotoxy(8,6);
    cout<< "Name \t\t\t: ";

    gotoxy(8,7);
    cout<< "Blood Type \t\t: ";

    gotoxy(8,8);
    cout<< "Phone no. \t\t: ";


    gotoxy(8,9);
    cout<<"Name of Organisation \t: ";


    gotoxy(36,5);
    getline(cin, LOCATION);
    transform(LOCATION.begin(), LOCATION.end(), LOCATION.begin(), ::toupper);

    gotoxy(36,6);
    fflush(stdin);// used to flush out the previous characters stored during compilation.
    getline(cin, NAME);
    transform(NAME.begin(), NAME.end(), NAME.begin(), ::toupper); //converts all string to upper case letters.

    gotoxy(36,7);
    fflush(stdin);
    getline(cin, BTYPE);
    transform(BTYPE.begin(), BTYPE.end(), BTYPE.begin(), ::toupper);

    gotoxy(36,8);
    fflush(stdin);
    getline(cin, PHONE);

    gotoxy(36,9);
    fflush(stdin);
    getline(cin, ORGANISATION);
    transform(ORGANISATION.begin(), ORGANISATION.end(), ORGANISATION.begin(), ::toupper);



        cout<<endl;
        string locations[19];

    //we stored all the locations in an array for future traversal use it will be easier.
        ifstream vertexOut;
        vertexOut.open("Location(vertex) List.txt");
        outVertex(vertexOut, locations);
        vertexOut.close();
        string loca=LOCATION;
        int x=0;
        transform(loca.begin(), loca.end(), loca.begin(), ::toupper);
        for(int i=1;i<=19;i++)
        {
            if(loca.compare(locations[i])==0)
            {
             x=i;
             break;
            }
        }


        if(x==0)
        {
        cout<<"SORRY YOU HAVE ENTERED A WRONG LOCATION (check spelling)"<<endl;
        }
        string bg=BTYPE;

        string bloodgrp[9]={"","O+","O-","A+","A-","B+","B-","AB+","AB-"};
        int bg1;int f2=0;



        for(int i=1;i<9;i++)
        {
        if(bg==bloodgrp[i])
        {
            bg1=i;
            f2=1;
        }
        }
        if(f2==0)
            cout<<"Invalid Blood Group"<<endl;

        file<<x<<" ";
        file<<bg1<<" ";
        file << LOCATION<< "\t";

        file << NAME<< "\t";
        file << BTYPE<< "\t";

        file << PHONE<< "\t";
        file << ORGANISATION<< "\t";
        file << endl;



    char op;
    cout<< "\tWant to enter more donors (Y/N) ??? : "; cin>>op;
    if(op=='Y'||op=='y')
    {
        system("CLS");
        DetailsToFile(file, LOCATION, NAME, BTYPE, ORGANISATION, PHONE);
    }
    else
        return;

}


struct locationNode{
    int index;
    float distWeight;
    int timeWeight;
    locationNode *next;
};
struct locationNode *newNode(int index, float distWeight, int timeWeight)
{
    locationNode *temp=new locationNode;
    temp->index=index;
    temp->distWeight=distWeight;
    temp->timeWeight=timeWeight;
    temp->next=NULL;
    return temp;
};
void sortedInsert(locationNode** head_ref, locationNode* new_node)
{
    locationNode* current;

    if (*head_ref == NULL || (*head_ref)->timeWeight >= new_node->timeWeight)
    {
        new_node->next = *head_ref;
        *head_ref = new_node;

    }
    else
    {

        current = *head_ref;
        while (current->next!=NULL &&
            current->next->timeWeight < new_node->timeWeight)
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

void print_locations(int prev[],int dest,int source, string locations[])
{
    stack <int> stk;
    int i=dest;
    while(i!=source)
    {
        stk.push(i);
        i=prev[i];
    }
    cout<<endl<<"\t "<<locations[source];
    while (!stk.empty())
    {
        cout << " -> " << locations[stk.top()];
        stk.pop();
    }

    return;
}
float find_dist_from_node(locationNode *Edge[],int i,int j)
{
    locationNode *temp=new locationNode;
    temp=Edge[i];
    while(temp!=NULL )
    {
        if (temp->index==j)
        {
            return temp->distWeight;
        }
        else
        {
            temp=temp->next;
        }
    }
    return 1000.0;
}
float shortest_dist(locationNode* Edge[],int source,int destination,float distance[],int previous[],int flg[],int c)
{
    float path,mini=1000.0;
    int new_source;
    distance[0]=0.0;
    flg[0]=1;
    flg[source]=1;
    if (c<17)
    {
        for (int i=0;i<19;i++)
        {

            if (source!=i)
            {

                if (flg[i]==0 && distance[i]>(distance[source]+find_dist_from_node(Edge,source,i)))
                {
                    distance[i]=distance[source]+find_dist_from_node(Edge,source,i);
                    previous[i]=source;
//marked as visited
                }
            }

        }

    //find_min_from_array
    for (int k =0;k<19;k++)
    {
        if (mini>distance[k] && flg[k]==0)
        {
            mini=distance[k];
            new_source=k;
        }
    }
    c++;

    path=shortest_dist(Edge,new_source,destination,distance,previous,flg,c);
    }
    path=distance[destination];
    return path;

}
int find_time_from_node(locationNode *Edge[],int i,int j)
{
    locationNode *temp=new locationNode;
    int dist_time;
    temp=Edge[i];
    while(temp!=NULL )
    {
        if (temp->index==j)
        {
            dist_time=temp->timeWeight;
            return dist_time;
        }
        else
        {
            temp=temp->next;
        }
    }
    return 1000;
}
int shortest_path(locationNode* Edge[],int source,int destination,int time[],int prev[],int flag[],int c)
{
    int mini=1000,new_source,path;
    time[0]=0;
    flag[0]=1;
    flag[source]=1;
    if (c<17)
    {
        for (int i=0;i<19;i++)
        {
            if (source!=i)
            {
                if (flag[i]==0 && time[i]>(time[source]+find_time_from_node(Edge,source,i)))
                {
                    time[i]=time[source]+find_time_from_node(Edge,source,i);
                    prev[i]=source;
                //marked as visited
                }
            }
        }
    //find_min_from_array
    for (int k =0;k<19;k++)
    {
        if (mini>time[k] && flag[k]==0)
        {
            mini=time[k];
            new_source=k;
        }
    }
    c++;
    path=shortest_path(Edge,new_source,destination,time,prev,flag,c);
    return path;
    }
    path=time[destination];
    return path;
}
void getDataFromFile(int loc,string bg)
{
    int flag=0;
    transform(bg.begin(), bg.end(), bg.begin(), ::toupper);
    string bloodgrp[9]={"","O+","O-","A+","A-","B+","B-","AB+","AB-"};
    int loc1,bg1;
    int f1=0,f2=0;

    loc1=loc;

    for(int i=1;i<9;i++)
    {
        if(bg==bloodgrp[i])
        {
            bg1=i;
            f2=1;
        }
    }
        if(f2==0)
            cout<<"Invalid Blood Group"<<endl;
    ifstream fin;
    fin.open("DONOR DETAILS.txt");
    string line,word;
    int i;char x;
    while(fin)
    {
        fin>>i;
        if(i==loc1)
        {
            fin>>i;
            if(i==bg1)
            {
                fflush(stdin);
                getline(fin,line);
                fflush(stdin);
                flag=1;
                cout<<endl<<line<<endl;
            }
            else
            {
                fflush(stdin);
                getline(fin,line);
                fflush(stdin);

            }
        }
        else
        {

            fflush(stdin);
            getline(fin,line);
            fflush(stdin);
        }
    }//end of while

    fin.close();
    if(flag==0)
    {
        cout<<endl<<"\t\tNO DONOR FOUND OF THAT BLOOD GROUP AT THIS LOCATION!!!"<<endl;
    }

}

vector<pair<int,int> > store;

bool sortbysec(const pair<int,int> &a,
              const pair<int,int> &b)
{
    return (a.second < b.second);
}

int visited[19];

void traversalOfEdge(struct locationNode* Edge1, int limit, struct locationNode* Edge[], int curr_index, int balanceTime)
{
    if(visited[curr_index]==0)
    {
    if(Edge1==NULL)
    {
        return;
    }
    if(balanceTime+Edge1->timeWeight<=limit)
    {
        if(store.size()<=0)
        {
            store.push_back(make_pair(Edge1->index, balanceTime+Edge1->timeWeight));
        }
        else{
        bool exist= false;
        for(vector<pair<int, int> > :: iterator it=store.begin(); it!=store.end();it++)
        {
              if(it->first == Edge1->index)
              {

                  exist=true;
                  if(it->second > balanceTime+Edge1->timeWeight)
                  {
                      it->second = balanceTime+Edge1->timeWeight;
                  }
              }
         }
         if(!exist)
         {
             store.push_back(make_pair(Edge1->index, balanceTime+Edge1->timeWeight));
             exist=false;
         }
        }
        traversalOfEdge(Edge1->next, limit, Edge, curr_index, balanceTime);

        visited[curr_index]=1;

        traversalOfEdge(Edge[Edge1->index], limit, Edge, Edge1->index, balanceTime+Edge1->timeWeight);

        visited[curr_index]=0;
    }


   }
   else
   {
    return;}
}
void printOURDONORDETAILS(string bg, string locations[]){
    cout<<endl<<"Locations available within the range you entered are: "<<endl;
    for(vector<pair<int, int> > :: iterator it=store.begin(); it!=store.end();it++)
        {
                cout<<endl<<locations[it->first]<<"\t"<<it->second<<" Minutes from here."<<endl;
                getDataFromFile(it->first, bg);
        }
}
int main()
{
    int time[19], prev[19],flag[19],shortest_time,flg[19],previous[19];
    float shortest_distance,distance[19];
    for (int i=0;i<19;i++)
    {
        time[i]=1000;
        flag[i]=0;
        flg[i]=0;
        distance[i]=1000.0;
    }

    Patient p;
    char y='y';
    while(y=='y'||y=='Y')
    {
        store.clear();
    system("cls");
    gotoxy(20,3);
    cout<<"BLOOD DONATION CAMP";
    gotoxy(25,5);
    cout<<"WELCOME!!!";
    gotoxy(16,8);
    cout<<"1. WANT TO BECOME A DONOR ? ";
    gotoxy(16,9);
    cout<<"2. WANT TO FIND BLOOD ? ";
    gotoxy(16,10);
    cout<<"3. ENTER 0 TO EXIT ";
    gotoxy(13,12);
    cout<<"ENTER YOUR CHOICE : ";
    int choice;
    gotoxy(34,12);
    cin>>choice;
    if(choice==1)
    {
        system("cls");
        ofstream file;
        file.open("DONOR DETAILS.txt", ios::app);
        string NAME, LOCATION, BTYPE, ORGANISATION, PHONE;
        DetailsToFile(file, LOCATION, NAME, BTYPE, ORGANISATION, PHONE);
        file.close();
    }
    else if(choice==2)
    {
        system("cls");
        float range;
        gotoxy(20,5);
        p.getPatient();
        cout<<"\n\tENTER RANGE(in Minutes) WITHIN WHICH WE WILL SEARCH THE DONORS(Max 500) : ";
        cin>>range;
        while(range>500)
        {
            cout<<endl<<"Limit Exceeded Enter Again: "<<endl;
            cin>>range;
        }


        locationNode* Edge[19];

    for(int i=0;i<19;i++)
    {
        Edge[i]=new locationNode;
        Edge[i]=NULL;

    }

   //STORING ALL THE VERTICES INTO ADJACENCY LIST;
    int vertexfrom1[]={2, 5, 6, 8, 7, 9, 14, 16, 1};
    float distfrom1[]={9.7, 10, 8.6, 2.9, 3.8, 11, 6.2, 12, 0};
    int timefrom1[]={30, 20, 16, 8, 25, 40, 10, 23, 0};
    for(int i=0;i<9;i++)
    {
        locationNode* new_node= newNode(vertexfrom1[i], distfrom1[i], timefrom1[i]);
        sortedInsert(&Edge[1], new_node);
    }
    int vertexfrom2[]={1, 3, 4, 5, 2};
    float distfrom2[]={9.7, 7.1, 3.4, 2.8, 0.0};
    int timefrom2[]={30, 15, 10, 5, 0};
    for(int i=0;i<5;i++)
    {
        locationNode* new_node= newNode(vertexfrom2[i], distfrom2[i], timefrom2[i]);
        sortedInsert(&Edge[2], new_node);
    }
    int vertexfrom3[]={2, 4, 17, 3};
    float distfrom3[]={7.1, 4.5, 7.1, 0.0};
    int timefrom3[]={15, 8, 19, 0};
    for(int i=0;i<4;i++)
    {
        locationNode* new_node= newNode(vertexfrom3[i], distfrom3[i], timefrom3[i]);
        sortedInsert(&Edge[3], new_node);
    }
    int vertexfrom4[]={2, 3, 4};
    float distfrom4[]={3.4, 4.5, 0.0};
    int timefrom4[]={10, 8, 0};
    for(int i=0;i<3;i++)
    {
        locationNode* new_node= newNode(vertexfrom4[i], distfrom4[i], timefrom4[i]);
        sortedInsert(&Edge[4], new_node);
    }
    int vertexfrom5[]={1, 2, 17, 6, 5};
    float distfrom5[]={10.0, 2.8, 4.0, 6.1, 0.0};
    int timefrom5[]={20, 5, 8, 18, 0};
    for(int i=0;i<5;i++)
    {
        locationNode* new_node= newNode(vertexfrom5[i], distfrom5[i], timefrom5[i]);
        sortedInsert(&Edge[5], new_node);
    }
    int vertexfrom6[]={1, 5, 18, 6};
    float distfrom6[]={8.6, 6.1, 4.2, 0.0};
    int timefrom6[]={16, 18, 13, 0};
    for(int i=0;i<4;i++)
    {
        locationNode* new_node= newNode(vertexfrom6[i], distfrom6[i], timefrom6[i]);
        sortedInsert(&Edge[6], new_node);
    }
    int vertexfrom7[]={1, 8, 9, 7};
    float distfrom7[]={3.8, 5.3, 7.6, 0.0};
    int timefrom7[]={25, 10, 10, 0};
    for(int i=0;i<4;i++)
    {
        locationNode* new_node= newNode(vertexfrom7[i], distfrom7[i], timefrom7[i]);
        sortedInsert(&Edge[7], new_node);
    }
    int vertexfrom8[]={1, 7, 8};
    float distfrom8[]={2.9, 5.3, 0.0};
    int timefrom8[]={8, 10, 0};
    for(int i=0;i<3;i++)
    {
        locationNode* new_node= newNode(vertexfrom8[i], distfrom8[i], timefrom8[i]);
        sortedInsert(&Edge[8], new_node);
    }
    int vertexfrom9[]={1, 7, 10, 9};
    float distfrom9[]={11, 7.6, 6.4, 0.0};
    int timefrom9[]={40, 10, 6, 0};
    for(int i=0;i<4;i++)
    {
        locationNode* new_node= newNode(vertexfrom9[i], distfrom9[i], timefrom9[i]);
        sortedInsert(&Edge[9], new_node);
    }
    int vertexfrom10[]={9, 14, 11, 10};
    float distfrom10[]={6.4, 7.5, 2.1, 0.0};
    int timefrom10[]={6, 9, 5, 0};
    for(int i=0;i<4;i++)
    {
        locationNode* new_node= newNode(vertexfrom10[i], distfrom10[i], timefrom10[i]);
        sortedInsert(&Edge[10], new_node);
    }
    int vertexfrom11[]={10, 12, 11};
    float distfrom11[]={2.1, 7, 0.0};
    int timefrom11[]={5, 13, 0};
    for(int i=0;i<3;i++)
    {
        locationNode* new_node= newNode(vertexfrom11[i], distfrom11[i], timefrom11[i]);
        sortedInsert(&Edge[11], new_node);

    }

    int vertexfrom12[]={11, 13, 12};
    float distfrom12[]={7, 6.4, 0.0};
    int timefrom12[]={13, 8, 0};
    for(int i=0;i<3;i++)
    {
        locationNode* new_node= newNode(vertexfrom12[i], distfrom12[i], timefrom12[i]);
        sortedInsert(&Edge[12], new_node);
    }
    int vertexfrom13[]={12, 13, 15};
    float distfrom13[]={6.4, 0, 6.3};
    int timefrom13[]={8, 0, 7};
    for(int i=0;i<3;i++)
    {
        locationNode* new_node= newNode(vertexfrom13[i], distfrom13[i], timefrom13[i]);
        sortedInsert(&Edge[13], new_node);
    }
    int vertexfrom14[]={1, 10, 14};
    float distfrom14[]={6.2, 7.5, 0.0};
    int timefrom14[]={10, 9, 0};
    for(int i=0;i<3;i++)
    {
        locationNode* new_node= newNode(vertexfrom14[i], distfrom14[i], timefrom14[i]);
        sortedInsert(&Edge[14], new_node);
    }
    int vertexfrom15[]={13, 15};
    float distfrom15[]={6.3, 0.0};
    int timefrom15[]={7, 0};
    for(int i=0;i<2;i++)
    {
        locationNode* new_node= newNode(vertexfrom15[i], distfrom15[i], timefrom15[i]);
        sortedInsert(&Edge[15], new_node);
    }
    int vertexfrom16[]={1, 16};
    float distfrom16[]={12, 0.0};
    int timefrom16[]={23, 0};
    for(int i=0;i<2;i++)
    {
        locationNode* new_node= newNode(vertexfrom16[i], distfrom16[i], timefrom16[i]);
        sortedInsert(&Edge[16], new_node);
    }
    int vertexfrom17[]={3, 5, 17};
    float distfrom17[]={7.1, 4, 0.0};
    int timefrom17[]={19, 8, 0};
    for(int i=0;i<3;i++)
    {
        locationNode* new_node= newNode(vertexfrom17[i], distfrom17[i], timefrom17[i]);
        sortedInsert(&Edge[17], new_node);
    }
    int vertexfrom18[]={6, 18};
    float distfrom18[]={4.2, 0.0};
    int timefrom18[]={13, 0};
    for(int i=0;i<2;i++)
    {
        locationNode* new_node= newNode(vertexfrom18[i], distfrom18[i], timefrom18[i]);
        sortedInsert(&Edge[18], new_node);
    }

    for(int i=0;i<19;i++)
    {visited[i]=0;}

    string locations[19];
    //we stored all the locations in an array for future traversal use it will be easier.
    ifstream vertexOut;
    vertexOut.open("Location(vertex) List.txt");
    outVertex(vertexOut, locations);
    vertexOut.close();

    string loca=p.location;
    int x=0;
    transform(loca.begin(), loca.end(), loca.begin(), ::toupper);
    for(int i=1;i<=19;i++)
    {
        if(loca.compare(locations[i])==0)
         {
             x=i;
             break;
         }
    }
    if(x==0)
    {
        cout<<"SORRY YOU HAVE ENTERED A WRONG LOCATION"<<endl;
    }
    else
    {

            traversalOfEdge(Edge[x], range, Edge, x, 0);
            sort(store.begin(), store.end(), sortbysec);
            printOURDONORDETAILS(p.bloodType, locations);

            cout<<endl<<endl<<endl<<"Enter your preferred location: "<<endl;
            string preferred;
            fflush(stdin);
            getline(cin, preferred);
            transform(preferred.begin(), preferred.end(), preferred.begin(), ::toupper);
            int ab=-1,k;
            for(int i=1;i<=19;i++)
            {
                if(preferred.compare(locations[i])==0)
                {
                    ab=i;
                    break;
                }

            }
            time[x]=0;
            prev[x]=0;
            shortest_time=shortest_path(Edge,x,ab,time,prev,flag,0);
            if (shortest_time>=1000 || shortest_time<0)
            {
                cout<<"WRONG LOCATION ENTERED"<<endl;
            }
            else
            {
                cout<<endl<<"Shortest Time to reach will be: "<<shortest_time<<" Minutes"<<endl;
                print_locations(prev,ab,x,locations);
                distance[x]=0.0;
                previous[x]=0;
                shortest_distance=shortest_dist(Edge,x,ab,distance,previous,flg,0);
                cout<<endl<<endl<<"Shortest Distance to reach will be: "<<shortest_distance<<" Kilometers"<<endl;
                print_locations(previous,ab,x,locations);
            }
    }
    }
    else if (choice==0)
    {
        return 0;
    }
    else
    {
        cout<<"\n\tPLEASE Enter a VALID CHOICE !!!"<<endl;
    }

    cout<<"\n \t\tWant to continue (y/n) : ";
    cin>>y;
    }
    return 0;
}

