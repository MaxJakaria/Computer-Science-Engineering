#include <windows.h>
#include <GL/glut.h>
#include <bits/stdc++.h>

int screenWidth = 1100;
int screenHeight = 600;
GLdouble xL, xR, yB, yT;

//:::::::::::::::::::::::::::::::::::::::Paddle properties:::::::::::::::::::::::::::::::::

float paddleWidth = 150.0f;
float paddleHeight = 20.0f;
float paddleX = (screenWidth - paddleWidth) / 2;
float paddleY = 20.0f;
float paddleSpeed = 15.0f;

//::::::::::::::::::::::::::::::::::::::::Ball properties::::::::::::::::::::::::::::::::::::

float ballRadius = 12.0f;
float ballX = paddleX + paddleWidth / 2;
float ballY = paddleY + paddleHeight + ballRadius;
float ballDirectionX = 1.0f;
float ballDirectionY = 1.0f;
float ballSpeed = 4.0f;

//:::::::::::::::::::::::::::::::::::::Bricks properties:::::::::::::::::::::::::::::::::::
int brickRows = 5;
int brickCols = 15;
struct Brick {
    float x;
    float y;
    float width;
    float height;
    bool isAlive;
};

std::vector<Brick> bricks;

//:::::::::::::::::::::::::::::::::Game state variables::::::::::::::::::::::::::::::::

bool isRunning = true;
bool gameWon = false;
bool gameLost = false;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void drawBoard(){
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex2f(screenWidth - 130, 90);
    glColor3f(0.0f, 0.1f, 0.0f);
    glVertex2f(screenWidth - 130, 40);
    glVertex2f(screenWidth - 10, 40);
    glVertex2f(screenWidth - 10, 90);
    glEnd();
}

float blockL_XL = 0.0;
float blockL_U = (screenHeight)/2 + 15;
float blockL_XR = screenWidth/5;
float blockL_D = (screenHeight)/2 - 15;
void drawL_Block(){
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex2f(blockL_XL, blockL_U);
    glColor3f(0.0f, 0.1f, 0.0f);
    glVertex2f(blockL_XL, blockL_D);
    glVertex2f(blockL_XR, blockL_D);
    glVertex2f(blockL_XR, blockL_U);
    glEnd();
}
float blockR_XL = screenWidth - screenWidth/5;
float blockR_U = (screenHeight)/2 + 15;
float blockR_XR = screenWidth;
float blockR_D = (screenHeight)/2 - 15;
void drawR_Block(){
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex2f(blockR_XL, blockR_U);
    glColor3f(0.0f, 0.1f, 0.0f);
    glVertex2f(blockR_XL, blockR_D);
    glVertex2f(blockR_XR, blockR_D);
    glVertex2f(blockR_XR, blockR_U);
    glEnd();
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

int score = 0;
int level = 1;
int levelup = 50;

// Function to update the score when a brick is destroyed
void updateScore() {
    score += 10; // Increment the score when a brick is destroyed
}

void updateLevel(){
    level += 1;
}

void drawScore() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(screenWidth - 110, 70);

    std::string scoreText = "Score: " + std::to_string(score);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    glRasterPos2f(screenWidth - 100, 50);
    std::string levelText = "Level: " + std::to_string(level);
    for (char c : levelText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , c);
    }
}

// ... (Previous functions remain unchanged)


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//:::::::::::::::::::::::::::::::::::::initGL:::::::::::::::::::::::::::::::::::

void initGL() {
    glClearColor(0.2, 0.2, 0.2, 1.0);
}

//::::::::::::::::::::::::::::::::::::::Initialize Bricks::::::::::::::::::::::::::;::::::::

void initializeBricks() {
    bricks.clear();
    float brickWidth = (screenWidth / brickCols)-1; // Distribute bricks evenly based on screen width
    float brickHeight = 40.0f;

    for (int i = 0; i < brickRows; i++) {
        for (int j = 0; j < brickCols; j++) {
            Brick brick;
            brick.x = j * brickWidth;
            brick.y = screenHeight - (i + 1) * brickHeight;
            brick.width = brickWidth;
            brick.height = brickHeight;
            brick.isAlive = true;
            bricks.push_back(brick);
        }
    }
}

//:::::::::::::::::::::::::::::::::::::::::Draw Paddle::::::::::::::::::::::::::::::::::::

void drawPaddle() {
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.7f, 0.2f);
    glVertex2f(paddleX, paddleY);
    glVertex2f(paddleX + paddleWidth, paddleY);
    glVertex2f(paddleX + paddleWidth, paddleY - paddleHeight);
    glVertex2f(paddleX, paddleY - paddleHeight);
    glEnd();
}

//:::::::::::::::::::::::::::::::::::::::::::Draw Ball::::::::::::::::::::::::::::::::::::::
float rndm1 = 0.0f, rndm2 = 0.8f, rndm3 = 0.2f;
void drawBall() {
    glColor3f(rndm2, rndm3,rndm1);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 360; i += 1) {
        float angle = i * 3.14159265359 / 180;
        glVertex2f(ballX + ballRadius * cos(angle), ballY + ballRadius * sin(angle));
    }
    glEnd();
}
//:::::::::::::::::::::::::::::::::::::::::Draw Bricks::::::::::::::::::::::::::::::::::::

void drawBricks() {
    for (auto &brick : bricks) {
        if (brick.isAlive) {
            glBegin(GL_QUADS);
            glColor3f(1.0f, 0.5f, 0.0f);
            glVertex2f(brick.x + 25, brick.y + 12);
            glColor3f(0.5f, 0.0f, 0.0f);
            glVertex2f(brick.x + brick.width, brick.y + 12);
            glColor3f(0.0f, 0.1f, 0.0f);
            glVertex2f(brick.x + brick.width, brick.y + brick.height);
            glColor3f(0.5f, 0.5f, 0.0f);
            glVertex2f(brick.x + 25, brick.y + brick.height);
            glEnd();
        }
    }
}

//::::::::::::::::::::::::::Update ball & bricks movement:::::::::::::::::::::

void update(int value) {
    if (!isRunning) {
        return;
    }

    ballX += ballSpeed * ballDirectionX;
    ballY += ballSpeed * ballDirectionY;

    if (ballX < ballRadius || ballX > screenWidth - ballRadius) {
        ballDirectionX = -ballDirectionX;
    }

    if (ballY > screenHeight) {
        ballY = screenHeight;
        ballDirectionY = -ballDirectionY;
    }

    if (ballY < 0) {
        gameLost = true;
        isRunning = false;
    }
    if (ballY < paddleY + ballRadius && ballX > paddleX - ballRadius && ballX < paddleX + paddleWidth + ballRadius) {
        ballDirectionY = -ballDirectionY;
    }
    if (ballY < blockL_U + ballRadius && ballY > blockL_D - ballRadius && ballX > blockL_XL - ballRadius && ballX < blockL_XR + ballRadius) {
        ballDirectionY = -ballDirectionY;
    }
    if (ballY < blockR_U + ballRadius && ballY > blockR_D - ballRadius && ballX > blockR_XL - ballRadius && ballX < blockR_XR + ballRadius) {
        ballDirectionY = -ballDirectionY;
    }


    for (auto &brick : bricks) {
        if (brick.isAlive) {
            if (ballX > brick.x && ballX < brick.x + brick.width && ballY > brick.y && ballY < brick.y + brick.height) {
                ballDirectionY = -ballDirectionY;
                srand(time(0));
                rndm1 =  rand()%2;
                rndm2 = rand()%3;
                rndm3 = rand()%4;
                brick.isAlive = false;
                updateScore();

                if(score==levelup){
                    updateLevel();
                    ballSpeed += 1;
                    paddleSpeed += 4;
                    levelup += 100;
                }
                bool allBricksDestroyed = true;
                for (const Brick &b : bricks) {
                    if (b.isAlive) {
                        allBricksDestroyed = false;
                        break;
                    }
                }
                if (allBricksDestroyed) {
                    gameWon = true;
                    isRunning = false;
                }
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

//::::::::::::::::::::::::::::::::::::::Display Function:::::::::::::::::::::::::::::::::::

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawPaddle();
    drawBricks();
    drawBoard();
    drawScore();
    drawBall();
    drawL_Block();
    drawR_Block();

    if (gameWon) {
        glColor3f(0.0f, 1.0f, 0.0f);
        glRasterPos2f(screenWidth / 2 - 60, screenHeight / 2);
        std::string winMessage = "You Win!";
        for (char c : winMessage) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    } else if (gameLost) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(screenWidth / 2 - 70, screenHeight / 2);
        std::string loseMessage = "Try Again..";
        for (char c : loseMessage) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    glutSwapBuffers();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::Keyboard:::::::::::::::::::::::::::::::::

void keyboard(int key, int x, int y) {
    if (!isRunning) {
        return;
    }

    switch (key) {
        case GLUT_KEY_LEFT:
            paddleX -= paddleSpeed;
            if (paddleX < 0) {
                paddleX = 0;
            }
            break;
        case GLUT_KEY_RIGHT:
            paddleX += paddleSpeed;
            if (paddleX > screenWidth - paddleWidth) {
                paddleX = screenWidth - paddleWidth;
            }
            break;
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::Reshape:::::::::::::::::::::::::::::::::::::::::

void reshape(int width, int height) {
    screenWidth = width;
    screenHeight = height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    paddleX = (screenWidth - paddleWidth) / 2;
    paddleY = 20.0f;
    ballX = paddleX + paddleWidth / 2;
    ballY = paddleY + paddleHeight + ballRadius;

    blockL_U = (screenHeight)/2 + 15;
    blockL_XR = screenWidth/5;
    blockL_D = (screenHeight)/2 - 15;
    blockR_XL = screenWidth - screenWidth/5;
    blockR_U = (screenHeight)/2 + 15;
    blockR_XR = screenWidth;
    blockR_D = (screenHeight)/2 - 15;

    initializeBricks();
}

//::::::::::::::::::::::::::::::::::::::::::::Main Fun:::::::::::::::::::::::::::::::::::::::::::::::

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Bounching Ball");

    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutReshapeFunc(reshape);

    initializeBricks();
    initGL();
    glutTimerFunc(0, update, 0);
    sndPlaySound("Sound.wav", SND_ASYNC);
    glutMainLoop();

    return 0;
}
