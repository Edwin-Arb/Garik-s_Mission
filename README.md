1. Полностью завершённая версия игры (Release)  
   Скачать игру можно по ссылке:  
   [Download](https://drive.google.com/file/d/1yjVqGRboA5Z5Ed9Bh2OUzuAo5k8eZpoB/view?usp=sharing)  
  
2. Документация проекта:  
   План проекта и техническое задание:  
   [Просмотреть документ](https://docs.google.com/document/d/1PVXK-9tD_Mn9Rr7zUbnBvGzsmgs-WWYV/edit)  
  
3. Набросок карты, созданный в программе Tiled:  
   [Открыть папку с файлами](https://drive.google.com/drive/folders/1-VhtIBmaszbhC2gfrfQN1rVQHUqxJjqk?hl=ru)  
  
4. Предварительная архитектура проекта (значительно изменялась в процессе разработки):  
   [Открыть папку с архитектурой](https://drive.google.com/file/d/1pYV8rEIesbFxOIa63RTtPwYFuE6dkEdM/view?usp=sharing)  
  
5. Краткое описание изменений:  
   - Были созданы менеджеры для управления коллизиями, звуком, спрайтами и анимациями.  
   - Добавлены абстрактные классы, которые унаследованы почти всеми классами игры для унифицированного управления и инициализации.  
   - Изначально большие классы были разбиты на более мелкие, чтобы облегчить поддержку и расширяемость кода.  
  
6. Полученные знания:  
   - Я активно использовал указатели, чтобы лучше понять работу с памятью и временем жизни объектов.  
   - Столкнулся с множественными утечками памяти и крашами, которые решил, изучив работу с памятью и корректное освобождение ресурсов. Также внедрил проверки, чтобы избежать   
     повторного удаления памяти или обращения к уже удалённой области.  
   - Освоил полиморфизм, forward declaration, а также основные принципы работы игровых движков.  
  
