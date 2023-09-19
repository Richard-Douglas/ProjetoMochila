#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SOIL/SOIL.h>

// Estrutura do objeto
struct Object {
    float x, y, size;
    int weight, value;
    int shape;
    bool collected = false;
    GLuint textureId;
};

Object objects[10]; // Número de objetos
float player_x = 0.0f, player_y = 0.0f; // Posição inicial do personagem
float currentWeight = 0.0f; // Peso atual na mochila
float currentValue = 0.0f; // Valor atual na mochila
const float maxWeight = 50.0f; // Peso máximo
int textCarreg = 0; // Verificar se as texturas já foram carregadas

bool checkOverlap(Object a, Object b, float minDistance) {
    float distance = std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
    return distance < minDistance;
}

bool checkCollision(float px, float py, float ox, float oy, float osize) {
    return px < ox + osize &&
           px + 0.1f > ox &&
           py < oy + osize &&
           py + 0.2f > oy;
}

class MyGlWindow : public Fl_Gl_Window {
public:
    float totalValue = 0.0f; // Novo: Valor total dos itens coletados
    GLuint backgroundTexture;
    GLuint objectTextures[3];
    GLuint characterTexture;

    MyGlWindow(int X, int Y, int W, int H, const char *L = 0) : Fl_Gl_Window(X, Y, W, H, L) {
    }

	void loadTextures() {
		if(textCarreg == 0){
			if (!characterTexture) {
				characterTexture = SOIL_load_OGL_texture("personagem.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
				if (!characterTexture) {
				    printf("SOIL loading error: '%s'\n", SOIL_last_result());
				}
			}

		
			if (!backgroundTexture) {
				backgroundTexture = SOIL_load_OGL_texture("background.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
				if (!backgroundTexture) {
				    printf("SOIL loading error: '%s'\n", SOIL_last_result());
				}
			}
			// Carrega as textura para cada objeto
			for (int i = 0; i < 10; ++i) {
		        if (!objects[i].collected) {
		            drawObject(objects[i], i);
		        }
		        
		        switch(i) {
					case 0:
						objects[i].textureId = SOIL_load_OGL_texture("textura1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
						break;
					case 1:
						objects[i].textureId = SOIL_load_OGL_texture("textura2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
						break;
					case 2:
						objects[i].textureId = SOIL_load_OGL_texture("textura3.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
						break;
					case 3:
						objects[i].textureId = SOIL_load_OGL_texture("textura4.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
						break;
					case 4:
						objects[i].textureId = SOIL_load_OGL_texture("textura5.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
						break;
					case 5:
						objects[i].textureId = SOIL_load_OGL_texture("textura6.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
						break;
					case 6:
						objects[i].textureId = SOIL_load_OGL_texture("textura7.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
						break;
					case 7:
						objects[i].textureId = SOIL_load_OGL_texture("textura8.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
						break;
					case 8:
						objects[i].textureId = SOIL_load_OGL_texture("textura9.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
						break;
					case 9:
						objects[i].textureId = SOIL_load_OGL_texture("textura10.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
						break;
					default:
						printf("SOIL loading error: '%s'\n", SOIL_last_result());
				}
		    }
		    
		    textCarreg = 1;
		}
	}

	// Ativa o OpenGl
	void setupOpenGL() {
		glEnable(GL_BLEND);  
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if (!valid()) {
		    valid(1);
		    glLoadIdentity();
		    glOrtho(-1.5, 1.5, -1.5, 1.5, -1, 1);
		}
		glClear(GL_COLOR_BUFFER_BIT);
	}

	// Carrega a textura do background
	void drawBackground() {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, backgroundTexture);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(-1.5, -1.5);
		glTexCoord2f(1.0, 0.0); glVertex2f(1.5, -1.5);
		glTexCoord2f(1.0, 1.0); glVertex2f(1.5, 1.5);
		glTexCoord2f(0.0, 1.0); glVertex2f(-1.5, 1.5);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	// Desenha o todos os objetos
	void drawObjects() {
		for (int i = 0; i < 10; ++i) {
		    if (!objects[i].collected) {
		        drawObject(objects[i], i);
		    }
		}
	}

	void drawInfo() {
        for (int i = 0; i < 10; ++i) {
	        switch(i) {
				case 0:{
					std::stringstream ssInfo;
				    ssInfo << "Fruta" << "| Peso: " << objects[i].weight << " / R$: " << objects[i].value;
				    glColor3f(1.0, 1.0, 1.0);
				    gl_font(FL_HELVETICA, 12);
				    gl_draw(ssInfo.str().c_str(), -1.45f, 1.4f - i * 0.1f);
					break;}
				case 1:{
					std::stringstream ssInfo;
				    ssInfo << "Bola" << "| Peso: " << objects[i].weight << " / R$: " << objects[i].value;
				    glColor3f(1.0, 1.0, 1.0);
				    gl_font(FL_HELVETICA, 12);
				    gl_draw(ssInfo.str().c_str(), -1.45f, 1.4f - i * 0.1f);
					break;}
				case 2:{
					std::stringstream ssInfo;
				    ssInfo << "Planta" << "| Peso: " << objects[i].weight << " / R$: " << objects[i].value;
				    glColor3f(1.0, 1.0, 1.0);
				    gl_font(FL_HELVETICA, 12);
				    gl_draw(ssInfo.str().c_str(), -1.45f, 1.4f - i * 0.1f);
					break;}
				case 3:{
					std::stringstream ssInfo;
				    ssInfo << "Celular" << "| Peso: " << objects[i].weight << " / R$: " << objects[i].value;
				    glColor3f(1.0, 1.0, 1.0);
				    gl_font(FL_HELVETICA, 12);
				    gl_draw(ssInfo.str().c_str(), -1.45f, 1.4f - i * 0.1f);
					break;}
				case 4:{
					std::stringstream ssInfo;
				    ssInfo << "Notebook" << "| Peso: " << objects[i].weight << " / R$: " << objects[i].value;
				    glColor3f(1.0, 1.0, 1.0);
				    gl_font(FL_HELVETICA, 12);
				    gl_draw(ssInfo.str().c_str(), -1.45f, 1.4f - i * 0.1f);
					break;}
				case 5:{
					std::stringstream ssInfo;
				    ssInfo << "Gameboy" << "| Peso: " << objects[i].weight << " / R$: " << objects[i].value;
				    glColor3f(1.0, 1.0, 1.0);
				    gl_font(FL_HELVETICA, 12);
				    gl_draw(ssInfo.str().c_str(), -1.45f, 1.4f - i * 0.1f);
					break;}
				case 6:{
					std::stringstream ssInfo;
				    ssInfo << "Moto" << "| Peso: " << objects[i].weight << " / R$: " << objects[i].value;
				    glColor3f(1.0, 1.0, 1.0);
				    gl_font(FL_HELVETICA, 12);
				    gl_draw(ssInfo.str().c_str(), -1.45f, 1.4f - i * 0.1f);
					break;}
				case 7:{
					std::stringstream ssInfo;
				    ssInfo << "Hamburguer" << "| Peso: " << objects[i].weight << " / R$: " << objects[i].value;
				    glColor3f(1.0, 1.0, 1.0);
				    gl_font(FL_HELVETICA, 12);
				    gl_draw(ssInfo.str().c_str(), -1.45f, 1.4f - i * 0.1f);
					break;}
				case 8:{
					std::stringstream ssInfo;
				    ssInfo << "Pizza" << "| Peso: " << objects[i].weight << " / R$: " << objects[i].value;
				    glColor3f(1.0, 1.0, 1.0);
				    gl_font(FL_HELVETICA, 12);
				    gl_draw(ssInfo.str().c_str(), -1.45f, 1.4f - i * 0.1f);
					break;}
				case 9:{
					std::stringstream ssInfo;
				    ssInfo << "Batata" << "| Peso: " << objects[i].weight << " / R$: " << objects[i].value;
				    glColor3f(1.0, 1.0, 1.0);
				    gl_font(FL_HELVETICA, 12);
				    gl_draw(ssInfo.str().c_str(), -1.45f, 1.4f - i * 0.1f);
					break;}
				default:
					break;
			}
	    }
        
        // Escreve as informações da mochila
        std::stringstream ssBackpack;
        ssBackpack << "Peso atual: " << currentWeight << "/" << maxWeight << " (" << (currentWeight / maxWeight) * 100 << "%)";
        ssBackpack << " ; R$->" << totalValue;
        glColor3f(1.0, 1.0, 1.0);
       	gl_font(FL_HELVETICA, 16);
        gl_draw(ssBackpack.str().c_str(), -1.4f, -1.4f);
	}

	// Desenha a textura do personagem
	void drawCharacter() {
		// Ativar textura
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, characterTexture);

		// Desenhar corpo do personagem
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(player_x, player_y);
		glTexCoord2f(1.0, 0.0); glVertex2f(player_x + 0.3f, player_y);
		glTexCoord2f(1.0, 1.0); glVertex2f(player_x + 0.3f, player_y + 0.6f);
		glTexCoord2f(0.0, 1.0); glVertex2f(player_x, player_y + 0.6f);
		glEnd();
		
		// Desativar textura
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

	}

	// Verifica colissões
	void handleCollisions() {
		for (int i = 0; i < 10; ++i) {
            if (!objects[i].collected && checkCollision(player_x, player_y, objects[i].x, objects[i].y, objects[i].size)) {
                if (currentWeight + objects[i].weight <= maxWeight) {
                    currentWeight += objects[i].weight;
                    totalValue += objects[i].value; // Novo: Atualizar o valor total
                    objects[i].collected = true;
                }
            }
        }
	}

	// Opera todas as funções de desenho
	void draw() {
		loadTextures();
		setupOpenGL();
		drawBackground();
		drawObjects();
		drawInfo();
		drawCharacter();
		handleCollisions();
		glDisable(GL_BLEND);
	}

    void drawObject(Object obj, int index) {
    	glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, obj.textureId);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(obj.x, obj.y);
		glTexCoord2f(1.0, 0.0); glVertex2f(obj.x + obj.size, obj.y);
		glTexCoord2f(1.0, 1.0); glVertex2f(obj.x + obj.size, obj.y + obj.size);
		glTexCoord2f(0.0, 1.0); glVertex2f(obj.x, obj.y + obj.size);
		glEnd();
		
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

        // Escreve os pesos dos objetos
		std::stringstream ssWeight;
		ssWeight << obj.weight << "kg | R$ " << obj.value;
		glColor3f(1.0, 1.0, 1.0); // Cor do texto
		gl_font(FL_HELVETICA, 16); // Fonte e tamanho
		gl_draw(ssWeight.str().c_str(), static_cast<float>(obj.x + 0.02), static_cast<float>(obj.y - 0.08)); // Posição do texto

    }
    
    
    // Inicializa os objetos
    void initializeObjects() {
        std::srand(std::time(0));
        for (int i = 0; i < 10; ++i) {
		    bool overlap;
		    do {
		        overlap = false;
		        objects[i].x = std::rand() % 200 / 100.0 - 1.0;
		        objects[i].y = std::rand() % 200 / 100.0 - 1.0;
		        for (int j = 0; j < i; ++j) {
		            if (checkOverlap(objects[i], objects[j], 0.4)) {
		                overlap = true;
		                break;
		            }
		        }
		    } while (overlap);

		    objects[i].size =  0.25;
		    objects[i].weight = (i + 1) * 2 + std::rand() % 5;
		    objects[i].value = std::rand() % 100 + 10;
		}
    }

};
int main(int argc, char **argv) {
    Fl_Window win(800, 600, "O Problema da Mochila");
    MyGlWindow mygl(10, 10, 780, 580);

    mygl.initializeObjects();
    win.end();
    win.show(argc, argv);

    float totalValue = 0.0f;

    while (Fl::check()) {
		// Move de acordo com a tecla
		if (Fl::event_key('w') && player_y + 0.2f < 1.5) player_y += 0.01f;
		if (Fl::event_key('s') && player_y > -1.5) player_y -= 0.01f;
		if (Fl::event_key('a') && player_x > -1.5) player_x -= 0.01f;
		if (Fl::event_key('d') && player_x + 0.1f < 1.5) player_x += 0.01f;

        // Atualiza o valor da mochila
        totalValue = 0.0f;
        for (int i = 0; i < 10; ++i) {
            if (objects[i].collected) {
                totalValue += objects[i].value;
            }
        }

        mygl.redraw();
    }

    return 0;
}
