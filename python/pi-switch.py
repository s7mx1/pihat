import logging
import logging.handlers
from jsonrpclib.SimpleJSONRPCServer import SimpleJSONRPCServer
import subprocess


def set_logger():
    logger = logging.getLogger('')
    logger.setLevel(logging.DEBUG)

    console = logging.StreamHandler()
    console.setLevel(logging.DEBUG)
    console.setFormatter(logging.Formatter('[%(asctime)s] %(message)s'))


    hdlr=logging.handlers.SysLogHandler(address="/dev/log")
    hdlr.setFormatter(logging.Formatter('[%(asctime)s] %(message)s'))
    hdlr.setLevel(logging.DEBUG)
    logger.addHandler(hdlr)

    logger.addHandler(console)
    return logger


def switch(parameters):
    command=["pihat"]
    for key in ("brand","channel","id","state","frequency"):
        command.append("--"+key+"="+parameters[key])
    process=subprocess.Popen(command, shell = False ,stdin=subprocess.PIPE,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    messages=[]
    while True:
        next_line = process.stdout.readline()
        if next_line == '' and process.poll() != None:
            break
        if not next_line.strip():
            continue
        else:
            messages.append(next_line)
    return (process.returncode,messages)


if __name__ == '__main__':
    global logger
    logger = set_logger()
    logger.debug('Running JSON-RPC server on port 8888')
    server = SimpleJSONRPCServer(('0.0.0.0', int(8888)),logger)
    server.register_introspection_functions()
    server.register_function(switch)
    server.serve_forever()
