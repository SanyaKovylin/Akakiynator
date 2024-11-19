#ifndef KAKIY_H_INCLUDED
#define AKAKIY_H_INCLUDED

typedef enum AkakiyPlachet{
    YA_V_PORYADKE = 0,
    NE_TO_IN_ANSWERS = 1,
    NICHEGO_NET = 2,
} a_err;

struct Buffer{
    char *buffer;
    size_t readptr;
    size_t len;
};



void  AktorRun   (void);
a_err AktorRecPrint     (FILE *fw, t_tree *Tree, int spaces);
a_err AktorPrintToFile(t_tree *ATree, const char *filesrc);
// TODO : Add Akak
a_err AktorParseSave   (const char* file, t_tree* Tree);
a_err AktorRecParse    (Buffer *src, t_tree* Tree);
a_err AktorParseAnswers(Buffer* src, t_tree *Tree);
a_err AktorGetName     (Buffer *src, char *name);
const int MaxNameSize = 100;

size_t Read (const char *src, char **Buffer);

a_err AktorRunGame       (t_tree *ATree);
a_err AktorRecursionGame (t_tree *Tree);
a_err AktorCheckAnswer   (t_tree *Tree);
a_err AktorAddAnswer     (t_tree *Tree);
a_err AktorRestart       (t_tree *Tree);

a_err AktorGetWord(char **word);
a_err AktorGetC(char *answer);

a_err AktorDefineGame(t_tree *Tree);
a_err AktorRecFind(char *obj, t_tree *Tree, t_path* Path);
#endif //AKAKIY_H_INCLUDED
































// #pragma
//     char que1[] = "Is an Animal?";
//     char que2[] = "Does know calculus?";
//     char que3[] = "Does CHUVSTVUET?";
//     char que4[] = "Lies that he is not a kvadrober?";
//
//     char ans1[] = "My cat";
//     char ans2[] = "Redkozubych";
//     char ans3[] = "Stepa";
//     char ans4[] = "Ded";
//     char ans5[] = "Sanya - Tea2taler";
//
//     #define Qnode(num) \
//         t_node Qnode##num = {};\
//         NodeCtor(&Qnode##num, que##num, sizeof(que##num));
//
//     Qnode(1);
//     Qnode(2);
//     Qnode(3);
//     Qnode(4);
//
//     #define Anode(num) \
//         t_node Anode##num = {};\
//         NodeCtor(&Anode##num, ans##num, sizeof(ans##num));
//
//     Anode(1);
//     Anode(2);
//     Anode(3);
//     Anode(4);
//     Anode(5);
//
//     Qnode1.left = &Anode1;
//     Qnode1.right = &Qnode2;
//     Qnode2.left = &Qnode3;
//     Qnode2.right = &Qnode4;
//     Qnode3.left = &Anode2;
//     Qnode3.right = &Anode3;
//     Qnode4.left = &Anode4;
//     Qnode4.right = &Anode5;
//
//     ATree.head = &Qnode1;
//     ATree.curr_node = ATree.head;
//
//     printf("%s",(char*) Anode1.value);
