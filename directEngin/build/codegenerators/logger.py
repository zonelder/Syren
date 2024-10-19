import logging
import sys

logLevel = logging.INFO

# Создаем глобальный логгер
log = logging.getLogger("BuildScriptLogger")
log.setLevel(logLevel)  # Уровень по умолчанию INFO
# Формат вывода сообщений

# Удаляем стандартные обработчики
for handler in log.handlers:
    if isinstance(handler, logging.StreamHandler):
        log.removeHandler(handler)

formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

# Вывод в консоль
console_handler = logging.StreamHandler(sys.stdout)
console_handler.setLevel(logLevel)  # Уровень по умолчанию INFO
console_handler.setFormatter(formatter)
log.addHandler(console_handler)

#end global definitions
