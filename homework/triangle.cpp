#include "../OPENGL.h"
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void corerender();
void rendersetting();
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout(location=0) in vec3 aPos;\n"
                                 "layout(location=1) in vec3 aColor;\n"
                                 "out vec3 ourColor;\n"
                                 "void main()\n"
                                 "{\n"
                                 " gl_Position =vec4(aPos,1.0);\n"
                                 "ourColor=aColor;\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec3 ourColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "FragColor=vec4(ourColor,1.0f);\n"
                                   "}\n\0";
float vertices[] = {
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // top
};
unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3};
unsigned int VBO, VAO;
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // rendersetting();
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        corerender();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
void rendersetting()
{
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR IN:" << infoLog << endl;
    }
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR IN Frag:" << infoLog << endl;
    }
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR IN LINK:" << infoLog << endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // 顶点坐标输入
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glUseProgram(shaderProgram);
}
// void DrawSphere_1(GLuint resolution)
// {
//     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//     GLdouble dTheta = 10 / resolution;
//     struct XY
//     {
//         GLdouble x, y;
//     };
//     auto a=new XY[2*resolution+1];
//     auto b=new XY[2*resolution+1];
//     GLdouble thetaZ=dTheta;
//     GLdouble thetaXY=0;
//     GLdouble az=cos(thetaZ);
//     GLdouble bz;
//     GLdouble rr=sin(thetaZ);
//     // glBegin
//     glBegin(GL_TRIANGLE_FAN);
//     glVertex3d(0,0,1);
//     for(auto i=0U;i<2*resolution;i++){
//         a[i].x=cos(thetaXY)*rr;
//         a[i].y=sin(thetaXY)*rr;
//         thetaXY+=dTheta;
//         glVertex3d(a[i].x,a[i].y,az);
//     }
//     a[2*resolution]=a[0];
//     glVertex3d(a[2*resolution].x,a[2*resolution].y,az);
//     glEnd();
//     for(auto i=1U;i<resolution-1;i++){
//         thetaZ+=dTheta;
//         bz=cos(thetaZ)*1;
//         rr=sin(thetaZ)*1;
//         thetaXY=dTheta/2*i;
//         glBegin(GL_TRIANGLES);
//         for(auto j=0U;j<2*resolution;j++){
//             b[j].x=cos(thetaXY)*rr;
//             b[j].y=sin(thetaXY)*rr;
//             thetaXY+=dTheta;
//             glVertex3d(a[j].x,a[j].y,az);
//             glVertex3d(a[j+1].x,a[j+1].y,az);
//             glVertex3d(a[j].x,b[j].y,bz);
//         }
//         b[2*resolution]=b[0];
//         glEnd();
//         auto tmp=a;
//         a=b;
//         b=tmp;
//         az=bz;
//     }
//     glBegin(GL_TRIANGLE_FAN);
//     glVertex3d(0,0,-1);
//     for(auto i=0U;i<2*resolution;i++){
//         glVertex3d(a[i].x,a[i].y,az);
//     }
//     glVertex3d(a[2*resolution].x,a[2*resolution].y,az);
//     glEnd();
//     delete[]a;
//     delete[]b;
// }
void corerender()
{
//    DrawSphere_1(1000);
}
