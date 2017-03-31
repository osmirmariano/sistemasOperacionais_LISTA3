# sistemasOperacionais_LISTA3


Um monitor de disciplina foi contratado pela UFT para ficar permanentemente disponível para os alunos. Sobretudo, quando não há alunos para serem atendidos ele descansa sobre a mesa. Assim, quando um aluno chega até ele e o encontra dormindo, ele o desperta para sanar sua dúvida e então vai embora. No corredor há um espaço para 4 alunos sentarem e esperarem por sua vez. Se o monitor está ocupado atendendo um aluno, o outro aluno que deseja atendimento senta na cadeira (se houver disponível). Caso não haja cadeiras disponíveis o estudante vai embora e volta mais tarde. Usando threads Posix, mutex locks e semáforos, implemente uma solução que coordene as atividades do monitor e dos estudantes.
 
Cada estudante e o monitor são threads. O estado do estudante varia entre jogar e procurar por ajuda. Caso ele procure por ajuda e não encontre cadeiras disponíveis ele volta a jogar. Coloque um número aleatório de milissegundos para as threads dormirem.
