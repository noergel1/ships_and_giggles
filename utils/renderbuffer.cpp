#include "renderbuffer.h"

Renderbuffer::Renderbuffer(int _width, int _height)
{
    glGenRenderbuffers(1, &m_renderbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbufferID); // now actually attach it

    // check if renderbuffer creation is finished
    if (glCheckFramebufferStatus(GL_RENDERBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::RENDERBUFFER:: Renderbuffer is not complete!" << std::endl;

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Renderbuffer::clear()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderbuffer::bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferID);
}

void Renderbuffer::unbind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferID);
}

void Renderbuffer::deleteBuffer()
{
    glDeleteRenderbuffers(1, &m_renderbufferID);
}
