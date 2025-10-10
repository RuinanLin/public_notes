# Example 4 - Using __getstate__ and __setstate__

import pickle

class Connection:
    def __init__(self, host, token):
        self.host = host
        self.token = token  # Sensitive info
        self.session = None

    def __getstate__(self):
        state = self.__dict__.copy()
        del state['token']  # Don't pickle the token
        return state

    def __setstate__(self, state):
        self.__dict__.update(state)
        self.token = None   # Reinitialize manually

conn = Connection('example.com', 'secret123')

serialized = pickle.dumps(conn)
restored = pickle.loads(serialized)

print(restored.__dict__)
# {'host': 'example.com', 'session': None, 'token': None}
