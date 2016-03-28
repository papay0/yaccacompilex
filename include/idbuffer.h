
// Initialise le idbuffer
void idbuffer_init();
// Ajoute un id au buffer
void idbuffer_addstr(char* identifier);
// Ajoute un pointeur quelconque au buffer
void idbuffer_add(void* identifier);
// Obtient l'id à la case i du buffer
void* idbuffer_get(int i);
// Obtient la taille du buffer.
int idbuffer_size();
// Selectionne le type des donnees contenues dans le type
void idbuffer_settype(int type);

// Retourne la taille du buffer de paramètres courant
int parambuffer_size();
// Crée et empile un nouveau buffer de paramètre 
void parambuffer_push();
// Ajoute un paramètre au buffer courant
void parambuffer_add(void* ptr);
// Obtient le paramètre i du buffer courant
void* parambuffer_get(int i);
// Dépile le buffer de paramètres courant
void parambuffer_pop();

