struct Node
{
	int val;
	Node* next;
};

int InsertList( Node* watchedHead, Node* element, int pos )
{
	Node* p;
	p = watchedHead;
	
	int i;
	i = 0;
	while ( i < pos )
	{
		p = p->next;		
		i = i + 1;
	}
	
	element->next = p->next;
	p->next = &element[0];

	return 0;
}

int main()
{
	Node node1, node2, node3, node4, node5;
	
	node1.val = 1;
	node2.val = 2;
	node3.val = 3;
	node4.val = 4;
	node5.val = 5;
	
	node1.next = &node2;
	node2.next = &node3;
	node3.next = &node4;
	node4.next = 0;
	node5.next = 0;
	Node*watchedpNode;
        watchedpNode=&node2;
	Node* watchedpHead;
	watchedpHead = &node1;
	
	InsertList( &node1, &node5, 2 );
	
	while ( watchedpHead != 0 )
	{
		write watchedpHead->val;
		write 1;
		watchedpHead = watchedpHead->next;		
	}
	
	return 0;
}