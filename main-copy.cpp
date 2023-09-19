#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SOIL/SOIL.h>

struct Object {
    float x, y, size;
    int weight, value;
    int shape;
    bool collected = false;
    GLuint textureId;
};

Object objects[10];
float player_x = 0.0f, player_y = 0.0f;
float currentWeight = 0.0f; // Peso atual na mochila
float currentValue = 0.0f; // Valor atual na mochila
const float maxWeight = 50.0f; // Peso máximo
int textCarreg = 0;

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
			// Load textures for objects here, similarly
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

	void drawObjects() {
		for (int i = 0; i < 10; ++i) {
		    if (!objects[i].collected) {
		        drawObject(objects[i], i); // Assume drawObject handles texture binding
		    }
		}
	}

	void drawInfo() {
		for (int i = 0; i < 10; ++i) {
            std::stringstream ssInfo;
            ssInfo << "Id" << i + 1 << ": Peso->" << objects[i].weight << " ; R$->" << objects[i].value;
            glColor3f(1.0, 1.0, 1.0);
            gl_font(FL_HELVETICA, 12);
            gl_draw(ssInfo.str().c_str(), -1.45f, 1.4f - i * 0.1f);
        }
        
        // Novo: Draw backpack information
        std::stringstream ssBackpack;
        ssBackpack << "Peso atual: " << currentWeight << "/" << maxWeight << " (" << (currentWeight / maxWeight) * 100 << "%)";
        ssBackpack << " ; R$->" << totalValue;
        glColor3f(1.0, 1.0, 1.0);
       	gl_font(FL_HELVETICA, 12);
        gl_draw(ssBackpack.str().c_str(), -1.4f, -1.4f);
	}

	void drawCharacter() {
		// Ativar textura
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, characterTexture);

		// Desenhar corpo do personagem
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(player_x, player_y);
		glTexCoord2f(1.0, 0.0); glVertex2f(player_x + 0.5f, player_y);
		glTexCoord2f(1.0, 1.0); glVertex2f(player_x + 0.5f, player_y + 1.0f);
		glTexCoord2f(0.0, 1.0); glVertex2f(player_x, player_y + 1.0f);
		glEnd();
		
		// Desativar textura
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

	}

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

    // ... (o resto do seu código para o método drawObject)

    void drawObject(Object obj, int index) {
    	glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, obj.textureId);
		glColor3f(1.0f, 1.0f, 1.0f); // Defina a cor como branco quando usar texturas
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(obj.x, obj.y);
		glTexCoord2f(1.0, 0.0); glVertex2f(obj.x + obj.size, obj.y);
		glTexCoord2f(1.0, 1.0); glVertex2f(obj.x + obj.size, obj.y + obj.size);
		glTexCoord2f(0.0, 1.0); glVertex2f(obj.x, obj.y + obj.size);
		glEnd();
		
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

        // Draw weight below the object
		std::stringstream ssWeight;
		ssWeight << obj.weight << "kg | R$ " << obj.value;
		glColor3f(1.0, 1.0, 1.0); // White color for the text
		gl_font(FL_HELVETICA, 16); // Set the font and size
		gl_draw(ssWeight.str().c_str(), static_cast<float>(obj.x + 0.02), static_cast<float>(obj.y - 0.08)); // Draw the text slightly offset from the object

    }
    
    void initializeObjects() {
        std::srand(std::time(0));
        for (int i = 0; i < 10; ++i) {
		    bool overlap;
		    do {
		        overlap = false;
		        objects[i].x = std::rand() % 200 / 100.0 - 1.0;
		        objects[i].y = std::rand() % 200 / 100.0 - 1.0;
		        for (int j = 0; j < i; ++j) {
		            if (checkOverlap(objects[i], objects[j], 0.2)) {
		                overlap = true;
		                break;
		            }
		        }
		    } while (overlap);

		    objects[i].size =  0.25;
		    objects[i].weight = (i + 1) * 2 + std::rand() % 5;
		    objects[i].value = std::rand() % 100 + 10;

		    if (i < 4) {
		        objects[i].shape = 0;
		    } else if (i < 7) {
		        objects[i].shape = 1;
		    } else {
		        objects[i].shape = 2;
		    }
		}
    }

};
int main(int argc, char **argv) {
    Fl_Window win(800, 600, "O Problema da Mochila");
    MyGlWindow mygl(10, 10, 780, 580);

    mygl.initializeObjects();  // Initialize the objects
    win.end();
    win.show(argc, argv);

    float totalValue = 0.0f; // Novo: Total value in the backpack

    while (Fl::check()) {
		// Move character with keys
		if (Fl::event_key('w') && player_y + 0.2f < 1.5) player_y += 0.01f;
		if (Fl::event_key('s') && player_y > -1.5) player_y -= 0.01f;
		if (Fl::event_key('a') && player_x > -1.5) player_x -= 0.01f;
		if (Fl::event_key('d') && player_x + 0.1f < 1.5) player_x += 0.01f;

        // Novo: Update total value in the backpack
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
