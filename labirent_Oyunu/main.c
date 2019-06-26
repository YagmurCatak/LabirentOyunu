#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct node_t
{
	int yol;
	struct node_t* left;
	struct node_t* right;
	struct node_t* top;
	struct node_t* bottom;
	int color;
} node;

typedef struct _turnPoint
{
	node* konum;
	int donusYonu;
} turnPoint;

typedef struct _stack {
	turnPoint* data;
	struct _stack* next;
} stack;

typedef node* nodePtr;

typedef struct _liste {
	nodePtr data;
	struct _liste* next;
} liste;

typedef liste* listePtr;

//global variables
stack *top = NULL;

listePtr kok = NULL;
int col = 0;
//endof global variables

//function prototypes
void push(turnPoint *data);
turnPoint* pop();
void matrixYazdir(nodePtr root);
int baslangicBitisKonumKontrol(nodePtr root, int bsutun, int bsatir);
int* yonBul(nodePtr currentNode);
int bittiMi(nodePtr root, nodePtr currentNode, nodePtr bitisElemani, int satir, int sutun, nodePtr baslangicElemani); //1 donerse basarili bitti, 0 donerse fail.
nodePtr elemaninaGit(nodePtr* root, int bsutun, int bsatir);
int stackBosMu();
void ilerle(nodePtr* currentNode, int yon);
int tersPreviousYon(int yon);
void yolmatrisOlustur(nodePtr root, nodePtr bitisElemani, nodePtr baslangicElemani);
void ListeDoldur(nodePtr currentNode, listePtr* kok);
void yolBelirle(nodePtr root);
void listedeVarMi(nodePtr* currentNode);
int* gidilenYonBul(nodePtr currentNode);
nodePtr listeOlustur(int **arr, int i, int j, int m, int n);
void linked(nodePtr* root, int m, int n);
//endof function prototypes


int main()
{
	int sutun = 10, satir = 10, random = 0;
	int i = 0, j = 0;
	int bsatir = 0, bsutun = 0;
	int bitisSatir = 0, bitisSutun = 0;

	srand(time(NULL));

	printf("satir ve sutun sayisini giriniz = ");
	scanf("%d %d", &sutun, &satir);

	if (sutun >= 15)
	{
		printf("Ekran boyutlari asilmistir.Lutfen satir ve sutun sayisini girin");
		scanf("%d %d", &sutun, &satir);
	}

	//2 boyutlu raskele matris olusuturldu.
	int **arr;
	arr = (int **)calloc(sizeof(int*), satir);
	for (i = 0; i < satir; i++)
	{
		arr[i] = (int*)calloc(sizeof(int), sutun);
	}

	for (i = 0; i < satir; i++)
	{
		for (j = 0; j < sutun; j++)
		{
			arr[i][j] = rand() % 2;
		}
	}

	nodePtr head = listeOlustur(arr, 0, 0, sutun, satir);

	for (i = 0; i < satir; i++)
	{
		for (j = 0; j < sutun; j++)
		{
			linked(&head, i, j);
		}
	}

	matrixYazdir(head);


	do {
	printf("baslangic noktasini giriniz:");
	scanf("%d %d",&bsutun,&bsatir);
	} while (!baslangicBitisKonumKontrol(head, bsutun, bsatir));

	do {
	printf("bitis noktasini giriniz:");
	scanf("%d %d",&bitisSutun,&bitisSatir);
	} while (!baslangicBitisKonumKontrol(head, bitisSutun, bitisSatir));

	nodePtr baslangicElamani = (nodePtr)calloc(sizeof(node),1);
	baslangicElamani = elemaninaGit(&head, bsutun, bsatir);

	nodePtr bitisElemani = (nodePtr)calloc(sizeof(node),1);
	bitisElemani = elemaninaGit(&head, bitisSutun, bitisSatir);

	nodePtr currentNode = NULL;
	currentNode = (nodePtr)calloc(sizeof(nodePtr),1);
	currentNode = elemaninaGit(&head, bsutun, bsatir);

	printf("\n\n");
//trace: buraya geldi.
	int previousYon = -1;
	int bitisFlagi = 0;
	int offset = 0;
	do {

	int sum=0;
	int* mumkunyol;
	mumkunyol = (int*)calloc(sizeof(int)*4,1);
	mumkunyol = yonBul(currentNode);

	int kararVerilenYon = -1;
	for(i=offset; i<4; i++)
	{
		if (i == tersPreviousYon(previousYon))
			continue;
		if(mumkunyol[i]==1)
		{
			kararVerilenYon = i;
			break;
		}
	}

	for(i=0; i<4;i++)
	{
		if (i == tersPreviousYon(previousYon))
			continue;
		sum = sum + mumkunyol[i];
	}

	if(sum>1)
	{
		turnPoint* node = (turnPoint*)calloc(sizeof(turnPoint),1);
		node->donusYonu = kararVerilenYon;
		node->konum = currentNode;
		//dallanma var.
		push(node);

		ilerle(&currentNode, kararVerilenYon);

		offset = 0;
	}
	else if(sum==1)
	{
		//gidilecek tek yon var.
		ilerle(&currentNode, kararVerilenYon);
		offset = 0;
	}
	else
	{
		turnPoint* nextNode = pop();
		currentNode = nextNode->konum;
		offset = nextNode->donusYonu;
	}

	if (bittiMi(head, currentNode, bitisElemani,satir,sutun,baslangicElamani) != -1)
	{
		bitisFlagi = 0;
	}
	else
	{
		bitisFlagi = 1;
	}
		previousYon = kararVerilenYon;
	} while(bitisFlagi);

	return 0;
}

void push(turnPoint *data)
{
	if (top == NULL)
	{
		top = (stack*)calloc(sizeof(stack), 1);
		top->data = data;
		top->next = NULL;
	}
	else
	{
		stack *temp = (stack*)calloc(sizeof(stack), 1);
		temp->data = data;
		temp->next = top;
		top = temp;
	}
}

turnPoint* pop()
{
	turnPoint* retvalue = top->data;
	stack* temp = top;
	top = top->next;
	free(temp);
	return retvalue;
}

int baslangicBitisKonumKontrol(nodePtr root, int bsutun, int bsatir)
{
	int i = 0, j = 0;
	nodePtr temp = root;

	for (i = 0; i<bsutun; i++)
	{
		temp = temp->right;
	}

	for (j = 0; j<bsatir; j++)
	{
		temp = temp->bottom;
	}

	if (temp->yol == 1)
	{
		return 1;
	}

	else
		return 0;

}

int* yonBul(nodePtr currentNode)
{
	int* yol = (int*)calloc(4 * sizeof(int), 1);
	if (currentNode->right != NULL)
	{
		if (currentNode->right->yol == 1)
		{
			yol[0] = 1;
		}
		else
			yol[0] = 0;
	}
	else
		yol[0] = 0;

	if (currentNode->left != NULL)
	{
		if (currentNode->left->yol == 1)
		{
			yol[1] = 1;
		}
		else
			yol[1] = 0;
	}
	else
		yol[1] = 0;

	if (currentNode->top != NULL)
	{
		if (currentNode->top->yol == 1)
		{
			yol[2] = 1;
		}
		else
			yol[2] = 0;
	}
	else
		yol[2] = 0;

	if (currentNode->bottom != NULL)
	{
		if (currentNode->bottom->yol == 1)
		{
			yol[3] = 1;
		}
		else
			yol[3] = 0;
	}
	else
		yol[3] = 0;

	return yol;
}

void matrixYazdir(nodePtr root)
{
	nodePtr Rp;

	nodePtr Dp = root;

	while (Dp) {
		Rp = Dp;

		while (Rp) {
			printf("%d ", Rp->yol);
			Rp = Rp->right;
		}
		printf("\n");
		Dp = Dp->bottom;
	}
}

//1 donerse basarili bitti, 0 donerse fail.
int bittiMi(nodePtr root, nodePtr currentNode, nodePtr bitisElemani, int satir, int sutun, nodePtr baslangicElemani)
{
	int *musaitYonler = yonBul(currentNode);
	int sum = 0;
	int i = 0;
	for (i = 0; i < 4; i++)
	{
		sum += musaitYonler[i];
	}

	if (currentNode == bitisElemani)
	{
		printf("\n YOL BULUNDU!\n");
		yolmatrisOlustur(root, bitisElemani, baslangicElemani);
		yolBelirle(root);
		matrixYazdir(root);
		return 1;
	}
	else if (stackBosMu() && sum == 0)
	{
		printf("\n YOL BULUNAMADI!");
		return 0;
	}
	else
		return -1;
}

int stackBosMu()
{
	if (top == NULL)
		return 1;
	else
		return 0;
}

nodePtr elemaninaGit(nodePtr* root, int bsutun, int bsatir)
{
	int i = 0, j = 0;
	nodePtr temp = *root;

	for (i = 0; i<bsutun; i++)
	{
		temp = temp->right;
	}
	for (j = 0; j<bsatir; j++)
	{
		temp = temp->bottom;
	}

	temp->color = 1;
	return temp;
}

void ilerle(nodePtr* currentNode, int yon)
{
	switch (yon) {

	case 0:
		*currentNode = (*currentNode)->right;
		printf("saga gitti\n");
		break;
	case 1:
		*currentNode = (*currentNode)->left;
		printf("sola gitti\n");
		break;
	case 2:
		*currentNode = (*currentNode)->top;
		printf("yukari gitti\n");
		break;
	case 3:
		*currentNode = (*currentNode)->bottom;
		printf("asagi gitti\n");
		break;
	}
	(*currentNode)->yol = 0;
	(*currentNode)->color = 1;
}

int tersPreviousYon(int yon)
{
	switch (yon) {
	case 0:
		return 1;
	case 1:
		return 0;
	case 2:
		return 3;
	case 3:
		return 2;
	}
}

void ListeDoldur(nodePtr currentNode, listePtr *kok)
{

	if ((*kok) == NULL)
	{
		(*kok) = (listePtr)calloc(sizeof(listePtr), 1);
		(*kok)->data = currentNode;
		(*kok)->next = NULL;
	}
	else
	{
		listePtr temp = *kok;
		listePtr yeniDugum = (listePtr)calloc(sizeof(listePtr), 1);
		yeniDugum->data = currentNode;
		yeniDugum->next = NULL;

		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = yeniDugum;
	}
}

void listedeVarMi(nodePtr* currentNode)
{
	listePtr temp = kok;
	while (temp != NULL)
	{
		if (*(currentNode) == (temp->data))
		{
			(*currentNode)->yol = 1;
			return;
		}
		else
		{
			(*currentNode)->yol = 0;
		}
		temp = temp->next;
	}
}

int* gidilenYonBul(nodePtr currentNode)
{
	//colora bagli bulacak
	int* yol = (int*)calloc(4 * sizeof(int), 1);
	if (currentNode->right != NULL)
	{
		if (currentNode->right->color == 1)
		{
			yol[0] = 1;
		}
		else
			yol[0] = 0;
	}
	else
		yol[0] = 0;

	if (currentNode->left != NULL)
	{
		if (currentNode->left->color == 1)
		{
			yol[1] = 1;
		}
		else
			yol[1] = 0;
	}
	else
		yol[1] = 0;

	if (currentNode->top != NULL)
	{
		if (currentNode->top->color == 1)
		{
			yol[2] = 1;
		}
		else
			yol[2] = 0;
	}
	else
		yol[2] = 0;

	if (currentNode->bottom != NULL)
	{
		if (currentNode->bottom->color == 1)
		{
			yol[3] = 1;
		}
		else
			yol[3] = 0;
	}
	else
		yol[3] = 0;

	return yol;
}

void yolmatrisOlustur(nodePtr root, nodePtr bitisElemani, nodePtr baslangicElemani)
{
	turnPoint* deneme[100];
	nodePtr currentNode = NULL;
	currentNode = (nodePtr)calloc(sizeof(nodePtr), 1);
	currentNode = baslangicElemani;
	ListeDoldur(currentNode, &kok);
	int yon = 0;
	int i = 0;
	int counter = 0;

	while (!stackBosMu())
	{
		turnPoint *dugum = pop();
		deneme[i++] = dugum;
	}
	counter = i;
	while (currentNode != bitisElemani)
	{
		int varMi = 0;
		for (i = 0; i<counter; i++)
		{
			if (currentNode == deneme[i]->konum)
			{
				varMi = 1;
				yon = deneme[i]->donusYonu;
			}
		}

		if (varMi == 1)
		{
			switch (yon) {
			case 0:
				currentNode = currentNode->right;
				break;
			case 1:
				currentNode = currentNode->left;
				break;
			case 2:
				currentNode = currentNode->top;
				break;
			case 3:
				currentNode = currentNode->bottom;
				break;
			}
			ListeDoldur(currentNode, &kok);
		}
		else
		{
			int yon = 0;
			int* gidilcekyon = (int*)calloc(sizeof(int) * 4, 1);
			gidilcekyon = gidilenYonBul(currentNode);

			for (i = 0; i<4; i++)
			{
				if (gidilcekyon[i] == 1)
				{
					yon = i;
				}
			}

			switch (yon)
			{
			case 0:
				currentNode = currentNode->right;
				break;
			case 1:
				currentNode = currentNode->left;
				break;
			case 2:
				currentNode = currentNode->top;
				break;
			case 3:
				currentNode = currentNode->bottom;
				break;
			}
			ListeDoldur(currentNode, &kok);
		}

	}
}

void yolBelirle(nodePtr root)
{
	nodePtr iter = root;
	nodePtr satirBasi = root;
	int i = 0, j = 0;
	int counter_r = 0;
	int counter_c = 0;
	while (satirBasi != NULL)
	{
		iter = satirBasi;
		counter_r++;
		do {
			listedeVarMi(&iter);
			iter = iter->right;
		} while (iter != NULL);

		satirBasi = satirBasi->bottom;
	}

}

nodePtr listeOlustur(int **arr, int i, int j,
	int m, int n)
{

	if (i > n - 1 || j > m - 1)
		return NULL;

	nodePtr temp = (nodePtr)calloc(sizeof(node), 1);
	temp->bottom = NULL;
	temp->top = NULL;
	temp->right = NULL;
	temp->left = NULL;
	temp->yol = -1;
	temp->color = 0;


	temp->yol = arr[i][j];
	temp->color = 0;
	temp->right = listeOlustur(arr, i, j + 1, m, n);
	temp->bottom = listeOlustur(arr, i + 1, j, m, n);
	return temp;
}

void linked(nodePtr* root, int m, int n)
{
	nodePtr temp = *root;
	int i = 0;
	int j = 0;

	for (i = 0; i < m; i++)
	{
		temp = temp->bottom;
	}

	for (j = 0; j < n; j++)
	{
		temp = temp->right;
	}

	nodePtr rightNode = temp->right;
	nodePtr downNode = temp->bottom;

	if (rightNode != NULL)
		rightNode->left = temp;
	if (downNode != NULL)
		downNode->top = temp;
}
