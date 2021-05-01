tests = [ {'description': 'Running',
	'steps': [ {'inputs': [('PINA', 0x00)],'iterations': 1}], # No actual input
	'expected': [('PORTB', 0x01)],
	},
	{'description': 'Running',
	'steps': [ {'inputs': [('PINA', 0x00)],'iterations': 1}], # No actual input
	'expected': [('PORTB', 0x02)],
	},
	{'description': 'Running',
	'steps': [ {'inputs': [('PINA', 0x00)],'iterations': 1}], # No actual input
	'expected': [('PORTB', 0x04)],
	},
	{'description': 'Running',
	'steps': [ {'inputs': [('PINA', 0x00)],'iterations': 1}], # No actual input
	'expected': [('PORTB', 0x01)],
	},
	{'description': 'Running',
	'steps': [ {'inputs': [('PINA', 0x00)],'iterations': 1}], # No actual input
	'expected': [('PORTB', 0x02)],
	},
	{'description': 'Running',
	'steps': [ {'inputs': [('PINA', 0x00)],'iterations': 1}], # No actual input
	'expected': [('PORTB', 0x04)],
	},
	{'description': 'Running',
	'steps': [ {'inputs': [('PINA', 0x00)],'iterations': 1}], # No actual input
	'expected': [('PORTB', 0x01)],
	},
	{'description': 'Running',
	'steps': [ {'inputs': [('PINA', 0x00)],'iterations': 1}], # No actual input
	'expected': [('PORTB', 0x02)],
	},
	{'description': 'Running',
	'steps': [ {'inputs': [('PINA', 0x00)],'iterations': 1}], # No actual input
	'expected': [('PORTB', 0x04)],
	},
    ]
watch = ['PORTB', 'L_STATE']

