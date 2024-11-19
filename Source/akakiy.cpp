#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "tree.h"
#include "akakiy.h"

void AktorRun(void){

    t_tree Akinator = {};
    TreeCtor(&Akinator, NULL, "Akinator");

    AktorParseSave("save1.json", &Akinator);

    printf("Do you want to enter a game mode?.");

    char ans1 = 0;
    AktorGetC(&ans1);
    if (ans1 == 'Y')
        AktorRunGame(&Akinator);

    printf("Do you want to enter a define mode?.");

    char ans2 = 0;
    AktorGetC(&ans2);

    if (ans2 == 'Y')
        AktorDefineGame(&Akinator);

    printf("Do you want to enter a compare mode?.");

    char ans3 = 0;
    AktorGetC(&ans3);

    if (ans3 == 'Y')
        AktorRunGame(&Akinator);

    AktorPrintToFile(&Akinator, "save1.json");
}

a_err AktorPrintToFile(t_tree *ATree, const char *filesrc){

    assert(ATree != NULL);
    assert(ATree->curr_node != NULL);
    assert(filesrc != NULL);

    ATree->curr_node = &ATree->head;

    FILE *fw = fopen(filesrc, "w");
    int spaces = 0;

    AktorRecPrint(fw, ATree, spaces);

    return YA_V_PORYADKE;
}

const int tabsp = 8;


a_err AktorRecPrint(FILE *fw, t_tree *Tree, int spaces){

    char *spacestring = (char*) calloc(spaces + 1, sizeof(char));
    memset(spacestring, ' ', spaces);

    char *spacestringlong = (char*) calloc(spaces + tabsp/2 + 1, sizeof(char));
    memset(spacestringlong, ' ', spaces + tabsp/2);

    if (NODE->left){

        printf("%s", spacestring);
        fprintf(fw, "\n %s{ ", spacestring);
        fprintf(fw, "\"%s\" : \n%s{", (char*)NODE->value, spacestringlong);
        fprintf(fw,"\n");

        t_node *curr_node = *Tree->curr_node;



        fprintf(fw, "%s\"yes\":", spacestringlong);
        *Tree->curr_node = curr_node->left;
        AktorRecPrint(fw, Tree, spaces + tabsp);
        fprintf(fw, ",\n");


        fprintf(fw, "%s\"no\": ", spacestringlong);
        *Tree->curr_node = curr_node->right;
        AktorRecPrint(fw, Tree, spaces + tabsp);
        fprintf(fw,"\n");


        fprintf(fw, "%s}\n%s}", spacestringlong, spacestring);
    }
    else{
        fprintf(fw, "\"%s\"", (char*)NODE->value);
    }

    free(spacestring);
    free(spacestringlong);

    return YA_V_PORYADKE;
}

size_t Read (const char *src, char **Buffer) {

    assert (src    != NULL);
    assert (Buffer != NULL);

    int fo = open (src, O_RDONLY | O_BINARY);

    struct stat st = {};

    stat (src, &st);
    _off_t readlen = st.st_size;

    *Buffer = (char*) calloc (readlen + 1, sizeof (char));

    size_t lenbuf = read (fo, *Buffer, readlen);
    return lenbuf;
}

a_err AktorParseSave(const char* file, t_tree* Tree){ //Need Ctor before

    assert(Tree != NULL);

    Buffer Source = {
        .buffer = NULL,
        .readptr = 0,
        .len = 0,
    };

    Source.len = Read(file, &Source.buffer);

    AktorRecParse(&Source, Tree);

    Tree->curr_node = &Tree->head;

    free(Source.buffer);

    return YA_V_PORYADKE;
}

#define readval src->buffer[src->readptr]

a_err AktorRecParse(Buffer *src, t_tree* Tree){

    char *name = (char*) calloc (MaxNameSize, sizeof(char));
    AktorGetName(src, name);

    for(;readval != '{' && readval != '}' && readval != ',';( src->readptr++));

    t_node *new_node = NULL;
    BuildNode(&new_node, name, strlen(name));

    *Tree->curr_node = new_node;

    if (readval == '{'){
        assert(!AktorParseAnswers(src, Tree));
    }

    free(name);

    return YA_V_PORYADKE;
}

a_err AktorParseAnswers(Buffer* src, t_tree *Tree){

    int ans_check = 0;
    t_node **node_save = Tree->curr_node;

    for (int i = 0; i < 2; i++){
        char ans[4] = {};
        AktorGetName(src, ans);

        if (!strcmp(ans, "no")){

            Tree->curr_node = &(*node_save)->right;
            AktorRecParse(src, Tree);
            ans_check += 1;

        }
        else if(!strcmp(ans, "yes")){

            Tree->curr_node = &(*node_save)->left;
            AktorRecParse(src, Tree);
            ans_check += 2;
        }
    }

    if (ans_check != 3)
        return NE_TO_IN_ANSWERS;
    return YA_V_PORYADKE;
}

a_err AktorGetName(Buffer *src, char *name){

    for (;readval != '\"'; src->readptr++);

    sscanf(src->buffer + src->readptr + 1, "%[^\"]", name);
    src->readptr += 2 + strlen(name);
    // printf("%s", name);
    return YA_V_PORYADKE;
}




a_err AktorRunGame(t_tree *ATree){

    printf("Start game\n");

    ATree->curr_node = &ATree->head;

    return AktorRecursionGame(ATree);
}

a_err AktorRecursionGame(t_tree *Tree){

    assert(Tree != NULL);
    assert(Tree->curr_node != NULL);
    assert(NODE != NULL);

    if (NODE->left == NULL) {
        return AktorCheckAnswer(Tree);
    }

    printf("Your Object %s\n", NAME);

    bool cor_input = false;

    while (!cor_input){

        // printf("[Y] Yes [N] No\n");

        char ans = 0;
        AktorGetC(&ans);

        switch (ans){
            case 'Y':{
                cor_input = true;
                Tree->curr_node = &NODE->left;
                break;
            }
            case 'N':{
                cor_input = true;
                Tree->curr_node = &NODE->right;
                break;
            }
            default:{
                printf("(%c)Incorrect input! Try again.\n", ans);
            }
        }
    }

    return AktorRecursionGame(Tree);
}


a_err AktorCheckAnswer(t_tree *Tree){

    assert(Tree != NULL);
    assert(Tree->curr_node != NULL);
    assert(NODE != NULL);

    printf("Is it %s ?\n", NAME);
    // printf("[Y] Yes [N] No\n");

    char ans = 0;
    AktorGetC(&ans);

    switch (ans){
        case 'Y':{
            printf("Great!\n");
            return AktorRestart(Tree);
        }
        case 'N':{
            printf("That's sad!\n");
            return AktorAddAnswer(Tree);
        }
        default:{
            printf("Ne Smeshno\n");
        }
    }
    return YA_V_PORYADKE;
}

a_err AktorGetC(char *answer){

    bool cor_input = false;
    char ans = 0;

    while (!cor_input){

        printf("[Y] Yes [N] No\n");

        scanf("%c", &ans);
        getchar();

        printf("input was: %c\n", ans);

        if (ans == 'Y' || ans == 'N')
            cor_input = true;
        else{
            printf("Incorrect input! Try again!\n");
        }
    }

    *answer = ans;

    return YA_V_PORYADKE;
}

a_err AktorGetWord(char **word){

   static char inword[MaxNameSize] = {};

   fgets(inword, MaxNameSize, stdin);

   assert(inword[MaxNameSize - 1] == '\0');

   int i = MaxNameSize - 1;
   for (; !inword[i]; i--);
   inword[i] = '\0';

   *word = inword;

   return YA_V_PORYADKE;
}

a_err AktorAddAnswer(t_tree *Tree){

    assert(Tree != NULL);
    assert(Tree->curr_node != NULL);
    assert(NODE != NULL);

    printf("What was it?\n");

    char *ans = NULL;
    AktorGetWord(&ans);

    printf("Please define the difference between %s and %s in question form"
    " such that it is true for the first one\n", ans, NAME);

    //TODO: fgets or getsymbol

    char *que = NULL;
    AktorGetWord(&que);
    // getchar();

    t_node *new_q = NULL;
    BuildNode(&new_q, que, strlen(que));

    t_node *new_ans = NULL;
    BuildNode(&new_ans, ans, strlen(ans));

    new_q->left = new_ans;
    new_q->right = NODE;
    NODE = new_q;

    return AktorRestart(Tree);
}

a_err AktorRestart(t_tree *Tree){

    printf("Do you want to restart the game?\n");
    // printf("[Y] Yes [N] No\n");

    char ans = 0;
    AktorGetC(&ans);

    switch (ans){
        case 'Y':{
            printf("Great!\n");
            return AktorRunGame(Tree);
        }
        case 'N':{
            printf("That's sad!\n");
            break;
        }
        default:{
            printf("Ne Smeshno\n");
        }
    }

    return YA_V_PORYADKE;
}

const int StartDepth = 10;

a_err AktorDefineGame(t_tree *Tree){

    printf("Please enter the object you want to find.");

    char *obj = NULL;
    AktorGetWord(&obj);

    Tree->curr_node = &Tree->head;

    t_path Path = {
        .Nodes = (t_node**) calloc (StartDepth, sizeof(t_node*)),
        .truth = (int*) calloc (StartDepth, sizeof(int)),
        .capacity = StartDepth,
        .depth = 0,
    };

    assert(Path.Nodes != NULL);
    assert(Path.truth != NULL);

    AktorRecFind(obj, Tree, &Path);

    // printf("%p", Path.Nodes[0]->value);

    for (int i = 0; i < Path.depth;i++){
        printf("%d: %s\n", Path.truth[i], Path.Nodes[i]->value);
    }




    return YA_V_PORYADKE;
}


a_err AktorRecFind(char *obj, t_tree *Tree, t_path* Path){

    printf("NAME : %s\n", NAME);
    printf("Obj : %s\n", obj);

    if (!stricmp(NAME, obj)){
        return YA_V_PORYADKE;
    }

    t_node *save = *Tree->curr_node;

    Path->Nodes[Path->depth++] = save;

    if (Path->capacity == Path->depth){
        Path->Nodes = (t_node**) Resize(Path->Nodes,sizeof(t_node*), Path->capacity, true);
    }

    printf("here");

    if (save->left){
        printf("left\n");
        Tree->curr_node = &save->left;
        Path->truth[Path.depth] = -1;
        if (!AktorRecFind(obj, Tree, Path))
            return YA_V_PORYADKE;
        printf("metod skipa\n");
    }

    if (save->right){
        printf("right");
        Path.truth[Path.depth] = 1;
        Tree->curr_node = &save->right;
        if (!AktorRecFind(obj, Tree, Path))
            return YA_V_PORYADKE;
    }

    Path->Nodes[--Path->depth] = NULL;
    Path->truth[Path->depth] = 0;
    printf("get proch\n");
    return NICHEGO_NET;
}

