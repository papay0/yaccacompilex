
// Initialise le idbuffer
void idbuffer_init();
// Ajoute un id au buffer
void idbuffer_add(char* const identifier);
// Obtient l'id à la case i du buffer
char* const idbuffer_get(int i);
// Obtient la taille du buffer.
int idbuffer_size();
// Selectionne le type des donnees contenues dans le type
void idbuffer_settype(int type);

