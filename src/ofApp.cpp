

#include "ofMain.h"
#define NUM 3


class ofApp : public ofBaseApp {
    
    ofShader shader;
    int bshow;
    
    ofVbo of_vao;
    ofBufferObject posVBO, colorVBO;

    ofVbo interLeaved_Vao;
    ofBufferObject triangleVBO;

    GLuint VBO, VAO;

    
    //--------------------------------------------------------------
    void setup()
    {
        
        shader.load("shader");
        GLuint posAttribute = shader.getAttributeLocation("position");
        GLuint colorAttribute = shader.getAttributeLocation("color");
        
        
        //1. 1VAO binds 2Vbos using oF API
        {
            ofVec3f vert[NUM];
            ofFloatColor colorvert[NUM];
            
            vert[0] = ofVec3f(0.5f, -0.5f, 0.0f);
            vert[1] = ofVec3f(-0.5f, -0.5f, 0.0f);
            vert[2] = ofVec3f(0.0f,  0.5f, 0.0f);
            
            
            colorvert[0] = ofFloatColor(1.0f, 0.0f, 0.0f, 1.0f);
            colorvert[1] = ofFloatColor(0.0f, 1.0f, 0.0f, 1.0f);
            colorvert[2] = ofFloatColor(0.0f, 0.0f, 1.0f, 1.0f);
            
            
            posVBO.allocate();
            posVBO.bind(GL_ARRAY_BUFFER);
            posVBO.setData(sizeof(ofVec3f) * 3, vert, GL_STATIC_DRAW);
            posVBO.unbind(GL_ARRAY_BUFFER);
            
            colorVBO.allocate();
            colorVBO.bind(GL_ARRAY_BUFFER);
            colorVBO.setData(sizeof(ofFloatColor) * 3, colorvert, GL_STATIC_DRAW);
            colorVBO.unbind(GL_ARRAY_BUFFER);
            
            of_vao.bind();
            of_vao.setVertexBuffer(posVBO, 3, 0);
            of_vao.setColorBuffer(colorVBO, 0); //setColorBuffer handles 4 float, so we may have to use ofFloatColor.
            of_vao.unbind();
        }
        
        
        
        //2. Interleaved Buffer using oF API
        {
            struct Particle
            {
                ofVec3f Pos;
                ofVec3f Col;
            };
            
            Particle particles[NUM];
            
            particles[0].Pos = ofVec3f(0.5f, -0.5f, 0.0f);
            particles[1].Pos = ofVec3f(-0.5f, -0.5f, 0.0f);
            particles[2].Pos = ofVec3f(0.0f,  0.5f, 0.0f);
            
            particles[0].Col = ofVec3f(1.0f, 0.0f, 1.0f);
            particles[1].Col = ofVec3f(0.0f, 1.0f, 0.5f);
            particles[2].Col = ofVec3f(1.0f, 0.0f, 1.0f);
            
            
            triangleVBO.allocate();
            triangleVBO.bind(GL_ARRAY_BUFFER);
            triangleVBO.setData(sizeof(Particle) * NUM, particles, GL_STATIC_DRAW);
            triangleVBO.unbind(GL_ARRAY_BUFFER);
            
            
            interLeaved_Vao.bind();
            interLeaved_Vao.setAttributeBuffer(posAttribute, triangleVBO, 3, sizeof(Particle), 0);
            interLeaved_Vao.setAttributeBuffer(colorAttribute, triangleVBO, 3, sizeof(Particle), 12);
            interLeaved_Vao.unbind();
        }
        
        
        
        
        //3. Raw OpenGL API (Interleaved Buffer)
        //thanks for Sonar Systems!! https://www.youtube.com/watch?v=EIpxcNl2WJU
        {
            GLfloat vertices[] = {
                // Positions         // Colors
                0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
                -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  // Bottom Left
                0.0f,  0.5f, 0.0f,   0.0f, 1.0f, 1.0f   // Top
            };
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            // Position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            // Color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            glBindVertexArray(0); // Unbind VAO
        }

        
        bshow = 1;
    }
    
    
    //--------------------------------------------------------------
    void update()
    {
        
    }
    
    
    //--------------------------------------------------------------
    void draw()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        shader.begin();
        
        if(bshow == 1)
        {
            of_vao.draw(GL_TRIANGLES, 0, 3);
        }
        
        else if(bshow == 2)
        {
            interLeaved_Vao.draw(GL_TRIANGLES, 0, 3);
        }
        
        else if(bshow == 3)
        {
            glBindVertexArray( VAO );
            glDrawArrays( GL_TRIANGLES, 0, 3 );
            glBindVertexArray( 0 );

        }
        
        shader.end();
        
        
        if(bshow == 1) ofDrawBitmapString("1VAO binds 2Vbos using oF API", 50, 50);
        if(bshow == 2) ofDrawBitmapString("Interleaved Buffer using oF API", 50, 50);
        if(bshow == 3) ofDrawBitmapString("Raw OpenGL API (Interleaved Buffer)", 50, 50);
    }
    
    
    //--------------------------------------------------------------
    void keyPressed(int key)
    {
        if(key == '1') bshow = 1;
        if(key == '2') bshow = 2;
        if(key == '3') bshow = 3;
    }
    
    //--------------------------------------------------------------
    void keyReleased(int key){
        
    }
    
};


//========================================================================
int main( ){
    
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4, 1);
    settings.width = 1280;
    settings.height = 720;
    settings.resizable = false;
    ofCreateWindow(settings);
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
    
}
