import jsonrpclib
from optparse import OptionParser


if __name__ == "__main__":
    p = OptionParser()
    parameters={}
    p.add_option('--server', '-s', dest="server", help="ip address of remote server")
    p.add_option('--port', '-p', dest="port", default="8888", help="port number of remote server")
    p.add_option('--brand', '-b', dest="brand", default="5", help="remote brand")
    p.add_option('--channel', '-c', dest="channel", default="7", help="remote channel")
    p.add_option('--id', '-i', dest="id", default="0", help="remote id")
    p.add_option('--state', dest="state", default="1", help="remote state: 0/1")
    p.add_option('--frequency', '-f', dest="frequency", default="0", help="remote frequency: 0 ->433.92MHz, 1-> 100MHz")
    options, arguments = p.parse_args()
    if options.server:
        server = str(options.server)
    if options.port:
        port = str(options.port)
    if options.brand:
        parameters["brand"] = str(options.brand)
    if options.channel:
        parameters["channel"] = str(options.channel)
    if options.id:
        parameters["id"] = str(options.id)
    if options.state:
        parameters["state"] = str(options.state)
    if options.frequency:
        parameters["frequency"] = str(options.frequency)

    if "server" not in locals():
        p.print_help()
        p.error("Please specify server ip address")

    server = jsonrpclib.Server('http://%s:%s' % (server,port) )
    switch = server.switch
    (return_code, messages) =switch(parameters)
    if return_code == 0:
        for line in messages:
            print line.rstrip()
    else:
        print "error occured\n"
        for line in messages:
            print line.rstrip()


