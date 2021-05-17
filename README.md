# MauroKart

## Requisitos objetivos
- [ ] No mínimo a aplicação deve incluir um modelo geométrico de complexidade igual ou superior ao modelo "cow.obj". Quanto maior a variedade de modelos geométricos, melhor.
- [X] Através da interação com o teclado e/ou mouse, o usuário deve poder controlar transformações geométricas aplicadas aos objetos virtuais (não somente controle da câmera).
- [ ] No mínimo a aplicação deve implementar uma câmera look-at e uma câmera livre.
- [ ] No mínimo um objeto virtual deve ser copiado com duas ou mais instâncias, isto é, utilizando duas ou mais `Model matrix` aplicadas ao mesmo conjunto de vértices. Exemplo: cubos desenhando todas partes do boneco como visto em aula.
- [X] No mínimo a aplicação deve utilizar três tipos de teste de intersecção. Estes testes devem ter algum propósito dentro da lógica da aplicação. Os testes de colisão devem ser implementados em um arquivo separado chamado `collisions.cpp`.
- [ ] No mínimo a aplicação deve incluir objetos com modelos de iluminação difusa (Lambert) e Blinn-Phong.
- [ ] No mínimo a aplicação deve incluir um objeto com os seguintes modelos de interpolação para iluminação: Gouraud, Phong.
- [ ] No mínimo dois objetos virtuais da aplicação devem ter as cores definidas atráves de textura em imagens (mínimo duas imagens distintas).
- [ ] No mínimo um objeto virtual da aplicação deve ter a movimentação definida através de uma curca de Bezier cúbica. Esse objeto deve se movimentar de forma suave ao longo do espaço em um caminho curvo.
- [X] Todas as movimentações de objetos (incluindo a câmera) devem ser computadas baseado no tempo (isto é, movimentações devem ocorrer sempre na mesma velocidade independente da velocidade da CPU onde o programa está sendo executado).

---
## Requisitos de projeto
- [ ] Máximo 15% de código pronto (identificar trechos por comentários com fonte).
- [ ] Aplicação deve possibilitar interação em tempo real (não pode ser lento a ponto de impactar negativamente a jogabilidade).
- [ ] Aplicação deve possuir algum objetivo e lógica de controle não-trivial.
- [X] Aplicação deve utilizar matrizes vistas em aula (`Model matrix, Projection matrix, View matrix`). Não deve ser feito uso de funções como: `gluLookAt(), gluOrtho2D(), gluPerspective(), gluPickMatrix(), gluProject(), gluUnProject(), glm::lookAt(), glm::ortho(), glm::perspective(), glm::pickMatrix(), glm::rotate(), glm::scale(), glm::translate()`

---
## Funcionalidades Extras (Opcionais)
- [ ] Efeitos sonoros;
- [ ] Sombras;
- [ ] Interface Gráfica (botões, etc.);
- [ ] Rasterização de texto com fontes diversas;

---
## Entregável
- [ ] hash-entrega.txt
- [ ] Binário compilado
- [ ] Relatório PDF
- [ ] Vídeo de 3 a 5 minutos com a apresentação do trabalho e demonstração da aplicação gráfica.