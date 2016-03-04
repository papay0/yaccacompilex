
// Initialise le idbuffer
void idbuffer_init();
// Ajoute un id au buffer
void idbuffer_addstr(char* const identifier);
// Ajoute un pointeur quelconque au buffer
void idbuffer_add(void* identifier);
// Obtient l'id à la case i du buffer
void* idbuffer_get(int i);
// Obtient la taille du buffer.
int idbuffer_size();
// Selectionne le type des donnees contenues dans le type
void idbuffer_settype(int type);


int tempaddr_init();
int tempaddr_lock();
void tempaddr_unlock(int addr);
