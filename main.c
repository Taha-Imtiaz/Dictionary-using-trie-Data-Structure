#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>


struct NODE
{
	bool end;
	char *meaning;
	struct NODE *next[26];
};

struct NODE * getNode()
{
	struct NODE *temp = (struct NODE *)malloc(sizeof(struct NODE));
	temp->end = false;
	int i;
	for(i = 0; i < 26; i++){
		temp->next[i] = NULL;
	}
	return temp;
}

void insert(struct NODE **root, char word[20], char *mean)
{
	struct NODE *cur = *root;
	int len = strlen(word);
	int i;
	for(i = 0; i < len - 1; i++)
        {
		int j = (int)word[i] - (int)'A';
		if(cur->next[j] == NULL)
		{
			cur->next[j] = getNode();
			cur->next[j]->end = false;
		}
		cur = cur->next[j];
	}
	cur->meaning = mean;
	cur->end = true;
}

bool search(struct NODE **root, char word[20])
{
	if(word[0] == '\0')
	{
		printf("ERROR: PLEASE ENTER SOMETHING!\n");
		return false;
	}
	int i;
	for(i = 0; i < strlen(word); i++){
		word[i] = toupper(word[i]);
	}
	struct NODE *cur = *root;
	int len = strlen(word);
	for(i = 0; i < len; i++){
		int j = (int)word[i] - (int)'A';
		if(cur->next[j] == NULL){
			return false;
		}
		cur = cur->next[j];
	}
	if(cur && cur->end){
		return true;
	}
}

void Read(struct NODE **root)
{
	FILE *m = fopen("meanings.txt", "r");
	char mean[4000][200];
	int i = 0;
	while(!feof(m)){
		fgets(mean[i], 200, m);
		i++;
	}
	fclose(m);
	FILE *w = fopen("trie.txt", "r");
	char word[4000][20];
	i = 0;
	while(!feof(w)){
		fgets(word[i], 20, w);
		insert(root, word[i], mean[i]);
		i++;
	}
	printf("\n\t\tAVAILABLE WORDS: %d", i - 1);
	fclose(w);
}

void AddWord(struct NODE **root)
{
	char newWord[15];
	printf("\n\t\tENTER A WORD TO BE ENTERED: ");
	scanf("%s", &newWord);
	if(newWord == '\0'){
		printf("\n\t\t-------------------------------------------------\n");
		printf("\t\tERROR: PLEASE ENTER SOMETHING!!\n");
		printf("\t\t-------------------------------------------------\n\n");
		return;
	}
	int i;
	for(i = 0; i < strlen(newWord); i++){
		newWord[i] = toupper(newWord[i]);
	}
	if(search(root, newWord)){
		printf("\n\t\t-------------------------------------------------\n");
		printf("\t\tTHIS WORD ALREADY EXISTS!\n");
		printf("\t\t-------------------------------------------------\n\n");
		return;
	}
	newWord[strlen(newWord)] = '\n';
	//===========================================================================================================================================//
	char newMean[100];
	printf("\n\t\tENTER IT'S MEANING: ");
	scanf("%s", &newMean);
	if(newMean == '\0')
        {
		printf("\n\t\t-------------------------------------------------\n");
		printf("\t\tERROR: PLEASE ENTER SOMETHING!!\n");
		printf("\t\t-------------------------------------------------\n\n");
		return;
	}
	//int len = strlen(newMean);
	char meaning[strlen(newMean)];
	for(i = 0; i < strlen(newMean); i++){
		meaning[i] = toupper(newMean[i]);
	}
	meaning[strlen(meaning)] = '\n';
	FILE *w = fopen("trie.txt", "a");
	if(!w){
		printf("ERROR: FILE DOES NOT EXIST!");
		exit(1);
	}
	FILE *m = fopen("meanings.txt", "a");
	if(!m){
		printf("ERROR: FILE DOES NOT EXIST!");
		exit(1);
	}
	fprintf(w, newWord);
	fclose(w);
	fprintf(m, meaning);
	fclose(m);
	insert(root, newWord, newMean);
	printf("\n\t\t-------------------------------------------------\n");
	printf("\t\tSUCCESSFULLY ADDED!\n");
	printf("\t\t-------------------------------------------------\n\n");
}

void relatedWords(struct NODE **root, char word[20]){
	FILE *f = fopen("trie.txt", "r");
	char wordA[4000][20];
	int i = 0;
	while(!feof(f)){
		fgets(wordA[i], 20, f);
		i++;
	}
	int j;
	for(j = 0; j < i; j++){
		if(strncmp(wordA[j], word, 2) == 0){
			printf("\t\t\t\t\t%s", wordA[j]);
		}
	}
	printf("\n\t\t\t\t\tNO FURTHER WORDS RELATED!");
}

void deleteWord(struct NODE **root){
	printf("\n\n\t\tENTER A WORD TO DELETE: ");
	char c[20];
	scanf("%s", &c);
	int i;
	for(i = 0; i < strlen(c); i++){
		c[i] = toupper(c[i]);
	}
	int lenC = strlen(c);
	if(search(root, c)){
		struct NODE *cur = *root;
		int in;
		for(i = 0; i < strlen(c); i++){
			in = (int)c[i] - (int)'A';
			cur = cur->next[in];
		}
		cur->meaning = NULL;
		cur->end = false;
		FILE *f = fopen("trie.txt", "r");
		char word[4000][20];
		i = 0;
		while(!feof(f)){
			fgets(word[i], 20, f);
			i++;
		}
		f = fopen("trie.txt", "w");
		int temp;
		int j;
		for(j = 0; j < i; j++){
			int q = strncmp(word[j], c, lenC - 1);
			if(q == 0){
				temp = j;
				continue;
			}
			fprintf(f, word[j]);
		}
		fclose(f);
		FILE *ff = fopen("meanings.txt", "r");
		char mean[4000][100];
		i = 0;
		while(!feof(ff)){
			fgets(mean[i], 100, ff);
			i++;
		}
		ff = fopen("meanings.txt", "w");
		for(j = 0; j < i; j++){
			if(j == temp){
				continue;
			}
			fprintf(ff, mean[j]);
		}
		fclose(ff);

		printf("\n\t\t-------------------------------------------------\n");
		printf("\t\tSUCCESSFULLY DELETED!\n");
		printf("\t\t-------------------------------------------------\n\n");
		return;
	}
	printf("\n\t\t-------------------------------------------------\n");
	printf("\t\tERROR: NO SUCH WORD FOUND!\n");
	printf("\t\t-------------------------------------------------\n\n");
}

void checkMeaning(struct NODE **root){
	printf("\n\t\tENTER A WORD TO CHECK IT'S MEANING: ");
	char word[20];
	scanf("%s", word);
	struct NODE *cur = *root;
	int len = strlen(word);
	int i;
	for(i = 0; i < len; i++){
		word[i] = toupper(word[i]);
	}
	for(i = 0; i < len; i++){
		int j = (int)word[i] - (int)'A';
		if(cur->next[j] == NULL){
			printf("\n\t\t-------------------------------------------------\n");
			printf("\t\tERROR: NO SUCH WORD FOUND!\n");
			printf("\n\t\tRELATED WORDS ARE:\n");
			relatedWords(root, word);
			printf("\n\t\t-------------------------------------------------\n\n");
			return;
		}
		cur = cur->next[j];
	}
	if(cur && cur->end){
		printf("\n\t\t-------------------------------------------------\n");
		printf("\t\tWORD: %s\n\n\t\tMEANING: %s", word, cur->meaning);
		printf("\t\t-------------------------------------------------\n\n");
	}
}

int main(int argc, char *argv[]) {
struct NODE *root = getNode();
	printf("\n\t\tPRECAUTIONS:"
		   "\n\t\t1. YOUR CHOICE MUST BE AN INTEGER!"
		   "\n\t\t2. USE '-' INSTEAD OF SPACE IN WORDS OR MEANINGS!\n");


	printf("\n\n\t\t*****************SPELL CHECKER*****************\n\n\n");
	int a;
	while(a != 5)
        {

		printf("\n\t\tENTER 1 TO CHECK SPELLING\n\t\tENTER 2 TO CHECK MEANING\n\t\tENTER 3 TO ADD A WORD\n\t\tENTER 4 TO DELTE A WORD\n\t\tENTER 5 TO EXIT\n\n");
		printf("\t\tYOUR CHOICE: ");
		scanf("%d", &a);
		switch(a){


			case 1:
			    //Read(&root);
				printf("\n\n\t\tENTER A WORD TO CHECK: ");
				char c[20];
				scanf("%s", &c);
				if(search(&root, c)){
					printf("\n\t\t-------------------------------------------------\n");
					printf("\t\tTHE SPELL YOU ENTERED IS CORRECT!\n");
					printf("\t\t-------------------------------------------------\n\n");
				}else{
					printf("\n\t\t-------------------------------------------------\n");
					printf("\t\tERROR: THE SPELL YOU ENTERED IS NOT CORRENT!\n\n\n\t\tRELATED WORDS ARE:\n");
					relatedWords(&root, c);
					printf("\n\t\t-------------------------------------------------\n\n");
				}
				break;
			case 2:
				checkMeaning(&root);
				break;
			case 3:
				AddWord(&root);
				break;
			case 4:
				deleteWord(&root);
				break;
			case 5:
				break;

			default:
				printf("\n\t\t-------------------------------------------------");
				printf("\n\t\tERROR: PLEASE SELECT ONLY FROM THE GIVEN OPTIONS!");
				printf("\n\t\t-------------------------------------------------\n\n");
				break;
		}
	}
	return 0;
}
