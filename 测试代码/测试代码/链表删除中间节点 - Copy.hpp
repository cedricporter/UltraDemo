struct Node
{
	int val;
	Node* next;
};



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
	node4.next = &node5;
	node5.next = 0;
	Node*watchedpNode;
        watchedpNode=&node2;
	Node* watchedpHead;
	watchedpHead = &node1;
        node3.next = &node5;
	
	return 0;
}