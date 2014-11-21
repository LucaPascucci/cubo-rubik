#ifndef IMAGE_LOADER_H_INCLUDED
#define IMAGE_LOADER_H_INCLUDED
 
//Classe che rappresenta un immagine
class Image {
    public:
        Image(char* ps, int w, int h);
        ~Image();

		/*Crea un array formato da (R1,G1,B1,R2,G2,B2,...) che indicano il colore di ogni pixel dell'immagine.
		* Il range dei componenti dell'array va da 0 a 255.
		* L'array parte dal pixel in basso a sinistra e scorre verso destra fino alla fine della righa.
		* Succesivamente sale di riga ed esegue lo stesso procedimento spiegato nella riga soprastante.
		*/
        
        char* pixels;
        int width;
        int height;
};
 
//Leggere una immagine bitmap da file
Image* loadBMP(const char* filename);
 
#endif