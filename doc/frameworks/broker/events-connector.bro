const broker_port: port = 9999/tcp &redef;
redef exit_only_after_terminate = T;
redef BrokerComm::endpoint_name = "connector";
global my_event: event(msg: string, c: count);
global my_auto_event: event(msg: string, c: count);

event bro_init()
	{
	BrokerComm::enable();
	BrokerComm::connect("127.0.0.1", broker_port, 1sec);
	BrokerComm::auto_event("bro/event/my_auto_event", my_auto_event);
	}

event BrokerComm::outgoing_connection_established(peer_address: string,
                                            peer_port: port,
                                            peer_name: string)
	{
	print "BrokerComm::outgoing_connection_established",
	      peer_address, peer_port, peer_name;
	BrokerComm::event("bro/event/my_event", BrokerComm::event_args(my_event, "hi", 0));
	event my_auto_event("stuff", 88);
	BrokerComm::event("bro/event/my_event", BrokerComm::event_args(my_event, "...", 1));
	event my_auto_event("more stuff", 51);
	BrokerComm::event("bro/event/my_event", BrokerComm::event_args(my_event, "bye", 2));
	}

event BrokerComm::outgoing_connection_broken(peer_address: string,
                                       peer_port: port)
	{
	terminate();
	}
