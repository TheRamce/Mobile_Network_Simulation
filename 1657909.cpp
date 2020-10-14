/* @Author
Student Name: Ramazan Yetişmiş
Student ID: 150190708
Date: 20.12.2019 */
  
#include <iostream> // C++ giriş/çıkış fonksiyonları için
#include <stdlib.h> // system("cls") fonksiyonu için
#include <iomanip>  // cin.ignore(), setw, vb. kullanımlar için
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;
struct MH
{
  int id;
  MH *next;
};

struct BS
{
  BS* child;
  MH* mh;;
  BS* next;
  int id;
 
  
};


struct Tree
{
  BS* root;
  int nodeCount;
  void create();
  BS* getParent(BS*,int);
  void addBS(BS* ,int);
  void addMH(MH*,int);
  void printBSH(BS*);
  void add_from_file(string filepath);
  bool containsMH(MH*,int);
  void deleteAll(BS*);
  void close();
  bool fullPath(BS* parent,int MHID);
  void read_message_from_file(string );
  void messagePath(int *);
  bool containsBS(BS*,int ID);
  BS* getBSparent(BS*,int);

};

struct StackNode
{
  int data; 
  StackNode* next; 
};

struct myStack
{
  StackNode *root;
  void create();
  StackNode* newNode(int data) ;
  bool isEmpty() ;
  void push( int data);
  int pop() ;
  void clear();
  string toString();
};




int LastBase[1];
typedef Tree Network;
Network connection;
myStack fullBasePath;
         
int main(int argc, char const *argv[])
{

      if (argc==3)
      {
         
          connection.create();
          connection.add_from_file(argv[1]);
          connection.read_message_from_file(argv[2]);
          //int x=6;
          //connection.messagePath(&x);

         //cout<< connection.containsBS(connection.root->child,4);
        // BS* a=connection.getBSparent(connection.root,6);
         //cout<<connection.getBSparent(connection.root,a->id)->parentIndex;
       
          connection.close();
          
      }
      else
      {
        cout<<"missing CommandLine Arg. "<<endl;
      }
      

      return 0;

}
//Create a the first node and make its ID 0
void Tree::create()
{

    root=new BS();
    root->id=0;
    root->next=NULL;
    root->mh=NULL;
    root->child=NULL;
    
    nodeCount=0;
    //cout<<"created"<<endl;
}
//recursive funct which finds the parent of the given BS
BS* Tree::getParent(BS* f, int searchID)
{
      BS* Found=NULL;
      //cout<<"f-> id"<<f->id<<endl;
      if (f->id==searchID)//if ids are equal return the parent

      {
       return f;
      }

      if (f->child)//if child exist
      {
            if ((Found=getParent(f->child,searchID))!=NULL)//call the funct for the child
            {
             return Found;
            }
            
      }

      if (f->next)//if next exist
      {
            if ((Found=getParent(f->next,searchID))!=NULL)//call the funct for the next
            {

              return Found;
            }
            
      }
      
      return NULL;//if cannot find return NULL
      
}
BS* Tree::getBSparent(BS* f,int searchID){

         BS* Found=NULL;
      //cout<<"f-> id"<<f->id<<endl;
      if (containsBS(f->child,searchID))//if ids are equal return the parent

      {
       return f;
      }

      if (f->child)//if child exist
      {
            if ((Found=getBSparent(f->child,searchID))!=NULL)//call the funct for the child
            {
             return Found;
            }
            
      }

      if (f->next)//if next exist
      {
            if ((Found=getBSparent(f->next,searchID))!=NULL)//call the funct for the next
            {

              return Found;
            }
            
      }
      
      return NULL;//if cannot find return NULL


}
//adds the BS to the tree 
void Tree::addBS(BS* addNode,int parentID)
{
  
  BS* addChild=new BS();
  addChild->id=addNode->id;
 
  addChild->mh=NULL;
  addChild->next=NULL;
  addChild->child=NULL;

    BS* parent=getParent(root,parentID);//by the help of this funct finds the parent of the given parent id
    if (getParent(root,addChild->id))
    {
      cout<<"Already exist you can not add dublicate"<<endl;
      return;
     
    }
    

          if (parent==NULL)
          {
              cout<<"Parent does not exist for "<<addChild->id<<endl;
              return;
          }

          if (parent->child==NULL)
          {
            parent->child=addChild;
            //cout<<"connection made to "<<addNode->id<<endl;
          }
          else
          {
            BS* child=parent->child;
            while (child->next)
            {
              child=child->next;
            }
            child->next=addNode;
            //cout<<"connection made "<<addNode->id<<endl;
            
          }
          
          
  


}
//prints the Tree recursively including MHs
void Tree::printBSH(BS* parent)
{
  
   if (parent){
       
        
      cout<<parent->id<<endl;
      
      MH* t=parent->mh;
      
      while (t)
      {
        cout<<t->id<<" "; 
        t=t->next;
      }
      cout<<endl;
       
      printBSH(parent->child);//call chils 
      printBSH(parent->next);//then next
    
    
   
       
    }    
      
}
//Adds the given MH to the given parent 
void Tree::addMH(MH* addNode,int parentID)
{
    //create the added node
      MH* child=new MH();
      child->id=addNode->id;
      child->next=NULL;
      if (parentID==0)
      {
       cout<<"can not add Central Base MHs"<<endl;
       return;
      }
      
      BS* parent=getParent(root,parentID);

      if (parent==NULL)
      {
        cout<<"no parent for "<<child->id <<endl;
        return;
      }
      //if it has no child make the first child
      if (parent->mh==NULL)
      {
        parent->mh=child;
        //cout<<"!MH concected "<<parent->mh->id<<endl;
      }
      else
      {
        //iterate te list and addlast
        MH* curr=parent->mh;
        while (curr->next)
        {
         curr=curr->next;
        }
        curr->next=child;
         //cout<<"!!MH concected "<<curr->next->id<<endl;
      }
      


      
      


}
//takes the path and opens the file
//while reading adds the network BSs to the Tree
void Tree::add_from_file(string filepath)
{	
      ifstream in(filepath);
      if(!in.is_open()){
        cout<<"ERROR: File could not been open"<<endl;
        return;
      }
      
    
      while(in.peek() != EOF )
      {		
        string name, id, parent;
        getline(in,name,' ');
        getline(in,id,' ');
        getline(in,parent,'\n');		

        if (!name.compare("BS"))
        {
        // cout<<"call add to BS"<<endl;
         //cout<<id <<" "<<parent<<endl;
         BS* newNode=new BS();
         newNode->id=stoi(id);
       
         newNode->child=NULL;
         newNode->mh=NULL;
         newNode->next=NULL;
         addBS(newNode,stoi(parent));
        }
        else if(!name.compare("MH"))
        {
           
          //cout<<"call add to MH"<<endl;
          MH* newMH=new MH();
          newMH->id=stoi(id);
          newMH->next=NULL;
          addMH(newMH,stoi(parent));
        }
        
        
        
       
      }
      in.close();
}
//iterates all MH until it finds the correctones
//if finds returns true else false
bool Tree::containsMH(MH* parent,int id){


     

     /* if (parent->id==id)
      {
        return true;
      }
      if (parent->next)
      {
        if (found=containsMH(parent->next,id))
        {
          return found;
        }
      }*/
      if (!parent)
      {
      return false;
      }
      else
      {
        MH* temp=parent;
        while (temp)
        {
          if (temp->id==id)
          {
            return true;
          }
          temp=temp->next;
          
        }
        
      }
      
      

      return false;

      
}
//deletes the whole tree by traversing  recursively
void Tree::deleteAll(BS* parent){

      if (parent){
       
        
     // cout<<parent->id<<endl;
    
     // cout<<endl;
       
      deleteAll(parent->child);
      deleteAll(parent->next);

        
      MH* t=parent->mh;
     
      
      while (t)
      {
       MH* temp=t;
        t=t->next;
        //cout<<"deleted "<<temp->id<<endl;
        delete temp;
      }
      //cout<<"deleted "<<parent->id<<endl;
      delete parent;
    }

}

//closes the Tree 
//by calling deleteAll() funct
void Tree::close(){
  if (!root->child)
  {
    
    delete root;
    return;
  }
  

  deleteAll(root);
}

//this is a bool funct which writes the path if 
//it finds the MH it returns and writes the path until return.
bool Tree::fullPath(BS* parent,int MHID){
      if (parent)
      {
        LastBase[0]=parent->id;
      cout<<parent->id<<" ";
      }
      bool retunrFlag=false;
    
      
      if (parent->id!=0)
      {
              if (containsMH(parent->mh,MHID))
              {
                return true;
              }

              if (parent->child)
              {
                if ((retunrFlag=fullPath(parent->child,MHID))==true)
                {
                  return retunrFlag;
                }
                
              }
              if (parent->next)
              {
                if ((retunrFlag=fullPath(parent->next,MHID))==true)
                {
                  return retunrFlag;
                }
                
              }
              return false;
      
        } else
        {
          return retunrFlag=fullPath(parent->child,MHID);
        }
    
  




}

//reads the message.txt and send messages to the MHs
void Tree::read_message_from_file(string filepath){
  if (!root->child)
  {
    return;
  }
  

      ifstream in(filepath);
      if(!in.is_open()){
        cout<<"ERROR: File could not been open"<<endl;
        return;
      }
      
      
      while(in.peek() != EOF )
      {		
        LastBase[0]=-1;
        string message, id;
        getline(in,message,'>');
        getline(in,id,'\n');
        if (id.empty())
        {
          //cout<<"null"<<endl;
          return;
        }
        
      cout<<"Traversing:";
     if (fullPath(root,stoi(id)))//if the MH is not available returns false
     {
       messagePath(LastBase);
       cout<<endl;
       cout<<"Message:"<<message<<" To:"<<fullBasePath.toString()<<"mh_"<<id;
       fullBasePath.clear();
      
     }
     else{
        cout<<endl;
       cout<<"Can not be reached the mobile host mh_";
       cout<<stoi(id)<<" at the moment";
     }
       cout<<endl;
        
        
        }
      in.close();
}
void Tree::messagePath(int *a){
  fullBasePath.create();
  fullBasePath.push(*a);
  BS* parent=getBSparent(root,*a);

  while (parent)
  {
    //cout<<parent->id;
    fullBasePath.push(parent->id);
    parent=getBSparent(root,parent->id);
  }
  
  }
  bool Tree::containsBS(BS* parent,int ID){

    if (parent==NULL )
    {
     return false;
    }
    
    BS* temp=parent;

    while (temp)
    {
 
    
       if (temp->id==ID)
       {
        return true;
       }
       temp=temp->next;
    }
    return false;
    
    
    
  }






//--------------------------Stack implementation---------------------------

  void myStack::create(){

     root=NULL;
}
StackNode* myStack::newNode(int data) {

  StackNode* stackNode = new StackNode(); 
    stackNode->data = data; 
    stackNode->next = NULL; 
    return stackNode; 
}

bool myStack::isEmpty() 
{ 
    if (root)
    {
     return false;
    }
    else
    {
      return true;
    }
    
    
    
} 

void myStack:: push( int data) 
{ 
    StackNode* stackNode = newNode(data); 
    stackNode->next = root; 
    root = stackNode; 
   
} 
int myStack::pop() 
{ 
    if (isEmpty()) 
        return -1; 
    StackNode* temp = root; 
    root = (root)->next; 
    int popped = temp->data; 
    delete(temp); 
  
    return popped; 
} 

void myStack::clear(){

  while (!isEmpty())
  {
   cout<<pop();
  }
  
  root=NULL;
  
}
string myStack::toString(){
    string s;
  while (!isEmpty())
  {
   s+=to_string(pop());
   s+=' ';
  }
  
   return s;
}

