void freeList(Node* head){
    Node* temp=head;
    while(head){
        head=head->next;
        free(temp);
        temp=head;
    }
}