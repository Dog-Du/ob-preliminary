import pymysql as mysql

from datetime import datetime
from ..base.module import BaseANN

class MiniOBVector(BaseANN):
    def __init__(self, metric, method_param):
        self._metric = metric
        self._unix_socket:str = method_param["unix_socket"]

        if metric == "euclidean":
            self._query = "SELECT id FROM items ORDER BY l2_distance(embedding,'[%s]') LIMIT %s"
        else:
            raise RuntimeError(f"unknown metric {metric}")

    def fit(self, X):
        conn = mysql.connect(unix_socket=self._unix_socket)
        cur = conn.cursor()
        print("before create table")
        cur.execute("CREATE TABLE items (id int, embedding vector(%d));" % X.shape[1])
        print("copying data: data size: %d..." % X.shape[0])
        for i, embedding in enumerate(X):
            cur.execute("insert into items values (%d, '[%s]')" % (i, ",".join(str(d) for d in embedding)))
            if i % 1000 == 0:
                print("%d copied" % i)

        index_start_time = datetime.now()
        print("{}: start create index!".format(index_start_time))

        if self._metric == "euclidean":
            cur.execute("CREATE VECTOR INDEX items_ivfflat_idx ON items (embedding) with(type=ivfflat, distance=l2_distance, lists=245, probes=5)")
        else:
            raise RuntimeError(f"unknown metric {self._metric}")

        print("{}: finish create index! build index in {}".format(datetime.now(), datetime.now()-index_start_time))
        self._cur = cur

    def set_query_arguments(self, n_probes):
        self._query_probes = n_probes
        return

    def query(self, v, n):
        self._cur.execute(self._query % ((",".join(str(d) for d in v), n)))
        return [int(id) for id, in self._cur.fetchall()]

    def __str__(self):
        return f"MiniOBVector()"