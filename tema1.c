#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

void CreateFind(ListOfFiles *headFile, ListOfDir *headDir,
	Directory *current, int adanc, int min_size, int max_size, char *arr3)
{
	ListOfFiles *aux = headFile;

	if (adanc >= 0) {
		adanc = adanc-1;
		while (aux) {
			char *p;

			//cautarea subsirului in continutul fisierelor

			p = strstr(aux->newFile->data, arr3);

			if (aux->newFile->size >= min_size &&
			aux->newFile->size <= max_size && p != NULL)
				printf("%s ", aux->newFile->name);
			aux = aux->next;
		}

		ListOfDir *temp = headDir;

		//parcurgerea in adancime a tuturor directoarelor copii

		while (temp) {
			CreateFind(temp->newDir->nextFile,
			temp->newDir->nextDir, temp->newDir, adanc, min_size,
			max_size, arr3);
			temp = temp->next;
		}
	}
}

//dealocarea spatiului alocat pentru elementele din lista de fisiere

void CreateRmallF(ListOfFiles **head)
{
	ListOfFiles *prev, *temp;

	prev = temp = (*head);
	while (temp != NULL) {
		temp = temp->next;
		free(prev->newFile->data);
		free(prev->newFile->name);
		free(prev->newFile);
		free(prev);
		prev = temp;
	}
	(*head) = NULL;
}

//dealocarea recursiva a memoriei alocate pentru
//elementele din lista de directoare

void CreateDeleteRec(ListOfDir **dir)
{
	if ((*dir)->newDir->nextFile)
		CreateRmallF(&(*dir)->newDir->nextFile);
	if ((*dir)->newDir->nextDir) {
		ListOfDir *p, *prev;

		p = prev = (*dir)->newDir->nextDir;
		while (p != NULL) {
			p = p->next;
			CreateDeleteRec(&prev);

			prev = p;
		}
	}

	(*dir)->newDir->nextDir = NULL;
	free((*dir)->newDir->name);
	free((*dir)->newDir);
	free(*dir);
}

//comanda recursiva de eliminare atat a directoarelor
//cat si a continutului acestora

void CreateRmdir(ListOfDir **headDir, Directory *current, char *arr2)
{
	ListOfDir *temp = *headDir
				, *previous = NULL;
	int ok = 0;

	//parcurgerea recursiva a listei de directoare
	//si a sublistelor aferente

	while (temp != NULL) {
		if (strcmp(temp->newDir->name, arr2) == 0) {
			if (previous == NULL) {
				*headDir = temp->next;
				current->nextDir = *headDir;
				if (temp->newDir->nextFile)
					CreateRmallF(&temp->newDir->nextFile);
			} else {

				previous->next = temp->next;
				if (temp->newDir->nextFile)
					CreateRmallF(&temp->newDir->nextFile);
			}
			ok = 1;
			if (temp->newDir->nextDir) {
				ListOfDir *p, *prev;

				p = prev = temp->newDir->nextDir;
				while (p != NULL) {
					p = p->next;
					CreateDeleteRec(&prev);
					prev = p;
				}
				temp->newDir->nextDir = NULL;
			}

			//dealocarea memoriei pentru directorul ce se doreste
			//a fi sters
			free(temp->newDir->name);
			free(temp->newDir);
			free(temp);
			break;
		}

		previous = temp;
		temp = temp->next;
	}
	if (ok == 0)
		printf("Cannot remove '%s': No such directory!\n", arr2);
}

void CreateRm(ListOfFiles **headFile, Directory *current, char *arr2)
{
	ListOfFiles *temp = *headFile
				, *previous = NULL;
	int ok = 0;

	while (temp != NULL) {
		if (strcmp(temp->newFile->name, arr2) == 0) {
			if (previous == NULL) {
				*headFile = temp->next;
				current->nextFile = *headFile;
			} else
				previous->next = temp->next;
			ok = 1;
			free(temp->newFile->data);
			free(temp->newFile->name);
			free(temp->newFile);
			free(temp);
			break;
		}

		previous = temp;
		temp = temp->next;
	}
	if (ok == 0)
		printf("Cannot remove '%s': No such file!\n", arr2);
}

void CreateTree(Directory *current, ListOfFiles *headFile,
				ListOfDir *headDir, int value)
{
	int copie_value = value;

	while (copie_value) {
		printf("    ");
		copie_value--;
	}

	printf("%s\n", current->name);

	ListOfFiles *aux = headFile;

	while (aux) {
		copie_value = value+1;
		while (copie_value) {
			printf("    ");
			copie_value--;
		}

		printf("%s\n", aux->newFile->name);
		aux = aux->next;
	}

	ListOfDir *temp = headDir;

	while (temp) {
		CreateTree(temp->newDir, temp->newDir->nextFile,
					temp->newDir->nextDir, value+1);
		temp = temp->next;
	}
}

void CreatePwd(Directory *current)
{
	char copie[256];

	strcpy(copie, current->name);

	if (strcmp(copie, "/") != 0) {
		CreatePwd(current->parentDir);
		printf("/%s", current->name);
	}
}

void CreateCd(Directory **current, ListOfFiles **headFile,
				ListOfDir **headDir, char *arr2)
{
	int ok = 0;
	ListOfDir *here;

	here = *headDir;

	if (strcmp(arr2, "..") == 0) {
		if ((*current)->parentDir  != NULL) {
			*headDir = (*current)->parentDir->nextDir;
			*headFile = (*current)->parentDir->nextFile;
			*current = (*current)->parentDir;
			ok = 1;
		}
	}

	while (here != NULL) {
		if (strcmp(here->newDir->name, arr2) == 0) {
			*current = here->newDir;
			ok = 1;
			*headDir = here->newDir->nextDir;
			*headFile = here->newDir->nextFile;
			break;
		}
			here = here->next;
	}

	if (ok == 0)
		printf("Cannot move to '%s': No such directory!\n", arr2);
}

//functie pentru adaugarea elementelor in lista de fisiere

void CreateTouch(Directory **current, ListOfFiles **headFile,
				char *arr2, char *arr3)
{
	ListOfFiles *temp;

	//alocarea memoriei pentru componentele pointerului cu care
	//este parcursa lista

	temp = (ListOfFiles *)malloc(sizeof(ListOfFiles));
	temp->newFile = (File *)malloc(sizeof(File));
	temp->newFile->name = (char *)malloc(sizeof(char) * 25);
	temp->newFile->data = (char *)malloc(sizeof(char) * 25);

	temp->newFile->size = strlen(arr3);
	strcpy(temp->newFile->name, arr2);
	strcpy(temp->newFile->data, arr3);

	if (*headFile == NULL) {
		temp->next = NULL;
		*headFile = temp;
		(*current)->nextFile = *headFile;
		return;
	}

	ListOfFiles *previous
				, *here;

	previous = here = *headFile;

	//ordonarea elementelor in mod lexicografic la introducerea
	//acestora in lista

	if (strcmp(arr2, previous->newFile->name) < 0) {
		temp->next = *headFile;
		*headFile = temp;
		(*current)->nextFile = *headFile;
		return;
	}

	while (here != NULL && strcmp(arr2, here->newFile->name) > 0) {
		previous = here;
		here = here->next;
	}

	previous->next = temp;
	temp->next = here;
}

//functie pentru adaugarea elementelor in lsita de directoare

void CreateMkdir(Directory **current, ListOfDir **headDir, char *arr2)
{
	ListOfDir *new;

	//alocarea memoriei pentru elementele componente ale pointerului
	//folosit in parcurgerea listei

	new = (ListOfDir *)malloc(sizeof(struct ListOfDir));
	new->newDir = (Directory *)malloc(sizeof(Directory));
	new->newDir->name = (char *)malloc(sizeof(char) * 25);

	strcpy(new->newDir->name, arr2);

	new->newDir->nextFile = NULL;
	new->newDir->nextDir = NULL;
	new->newDir->parentDir = *current;

	if (*headDir == NULL) {
		new->next = NULL;
		*headDir = new;

		(*current)->nextDir = *headDir;
		return;
	}

	ListOfDir *previous, *here;

	previous = here = *headDir;

	//compararea numelor pentru ordonarea acestora in mod lexicografic

	if (strcmp(arr2, previous->newDir->name) < 0) {
		new->next = *headDir;
		*headDir = new;
		(*current)->nextDir = *headDir;
		return;
	}

	while (here != NULL && strcmp(arr2, here->newDir->name) > 0) {
		previous = here;
		here = here->next;
	}

	previous->next = new;
	new->next = here;
}

//functie pentru afisarea listei de fisiere

void PrintFiles(ListOfFiles *cap)
{
	if (cap != NULL) {
		ListOfFiles *p = cap;

		while (p != NULL) {
			printf("%s ", p->newFile->name);
			p = p->next;
		}
	}
}

//functie pentru afisarea listei de directoare

void PrintDirs(ListOfDir *cap)
{
	if (cap != NULL) {
		ListOfDir *p = cap;

		while (p != NULL) {
			printf("%s ", p->newDir->name);
			p = p->next;
		}
	}
}

//functie pentru crearea directorului radacina si pentru alocarea memoriei
//necesare elementelor componente acestuia

void CreateRoot(Directory **root, Directory **current)
{
	(*root) = (Directory *) malloc(sizeof(struct Directory));
	(*root)->name = (char *) malloc(sizeof(char) * 4);
	(*root)->name[0] = '/';
	(*root)->nextFile = NULL;
	(*root)->nextDir = NULL;
	(*root)->parentDir = NULL;

	*current = *root;
}


int main(void)
{
	char sentence[256]
		, start[10] = "create fs"
		, end[10] = "delete fs"
		, *newLine
		, dels[] = "\n \0"
		,  **words;
	int lengthWords
		, adanc
		, min_size
		, max_size;

		//root - pointer catre directorul radacina
	struct Directory *root
		//current - pointer catre directorul curent
					, *current = NULL;
	struct ListOfDir *headDir = NULL;
	struct ListOfFiles *headFile = NULL;

	//citirea comenzilor in minishell-ul creat

	fgets(sentence, '\n', stdin);

	if (strcmp(sentence, start) == 0) {
		//crearea directorului radacina

		CreateRoot(&root, &current);
		fgets(sentence, '\n', stdin);

		//bucla pentru verificarea comenzilor introduse

		while (strcmp(sentence, end) != 0) {
			lengthWords = 0;
			fgets(sentence, 256, stdin);
			char aux[256];

			for (int i = 0; i < strlen(sentence); i++) {
				if (sentence[i] == '\n')
					sentence[i] = '\0';
			}
			strcpy(aux, sentence);

			//separarea cuvintelor din propozitie pentru obtinerea
			//comenzilor separate de parametrii

			newLine = strtok(sentence, dels);
			while (newLine != NULL) {
				lengthWords++;
				newLine = strtok(NULL, dels);
			}

			words = (char **)malloc(sizeof(char *) * lengthWords);

			int i = 0;

			strcpy(sentence, aux);
			newLine = strtok(aux, dels);
			while (newLine != NULL) {
				words[i] =
				(char *) malloc(sizeof(char) * 80);
				strcpy(words[i++], newLine);
				newLine = strtok(NULL, dels);
			}



			if (strcmp(words[0], "touch") == 0) {
				CreateTouch(&current, &headFile
					, words[1], words[2]);
			} else if (strcmp(words[0], "mkdir") == 0) {
				CreateMkdir(&current
						, &headDir, words[1]);
			} else if (strcmp(words[0], "ls") == 0) {
				PrintFiles(headFile);
				PrintDirs(headDir);
				printf("\n");
			} else if (strcmp(words[0], "pwd") == 0) {
				if (current) {
					if (strcmp(current->name, "/") == 0)
						printf("%s", current->name);
					else
						CreatePwd(current);
					printf("\n");
				}
			} else if (strcmp(words[0], "cd") == 0)
				CreateCd(&current, &headFile
					, &headDir, words[1]);
			else if (strcmp(words[0], "tree") == 0)
				CreateTree(current, headFile, headDir, 0);
			else if (strcmp(words[0], "rm") == 0)
//printf("imi dau delete la viata\n");
				CreateRm(&headFile, current, words[1]);
			else if (strcmp(words[0], "rmdir") == 0)
				CreateRmdir(&headDir, current
			, words[1]);
			else if (strcmp(words[0], "find") == 0) {
				//transformarea parametrilor in valori int
				adanc = atoi(words[1]);
				min_size = atoi(words[2]);
				max_size = atoi(words[3]);
				CreateFind(headFile, headDir, current,
					adanc, min_size, max_size, words[4]);
				printf("\n");
			}

			//dealocarea memoriei pentru cuvintele
			//rezultate la separare
			for (i = 0; i < lengthWords; i++)
				free(words[i]);
			free(words);
		}

		//dealocarea memoriei alocate pentru directorul radacina

		if (root) {
			if (root->name)
				free(root->name);
			if (root->nextFile)
				free(root->nextFile);
			if (root->nextDir)
				free(root->nextDir);
			if (root)
				free(root);
		}
	}

	return 0;
}
