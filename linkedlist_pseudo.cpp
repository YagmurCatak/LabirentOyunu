node 
{
    int data, 
    node* next,
    node* down,
    node* up,
    node* previous
}


row: 3
column: 4

12 tane random sayı. 

x11 x12 x13 x14
x21 x22 x23 x24
...
...

for (0;column)
{
    for(0;row)
    {
        int deg = rand();
        node* temp ;
        temp->data = deg;
        temp->next = null;
        temp->down = null;
        listeEkle(&head, temp, yanina/altina);
    }
}

listeEkle()
{
    if (yanila/altina)
    {
        head->next = temp;
        temp->previous = head;

    }
    head->down = temp;
    temp->up = head;
}