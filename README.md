# Destruindo Processos

Implementação do relacionamento de processos utilizados pelos Sistemas Operacionais e políticas de exclusão existentes.<br/>

Políticas de exclusão implementadas:<br/>
• Processos que têm filhos nãoo podem ser excluídos.<br/>
• Quando processos que têm filhos são excluídos, toda a subárvore a partir do ponto de exclusão deve ser excluída.<br/>
• Quando processos que têm filhos são excluídos, os filhos devem ser ligados ao vértice superior (avô).<br/>
