import os
import subprocess
import time
import tempfile

exe_path = # Path to the cpp executable
source_matrix_path = # Path to the source matrices to be ordered
output_path = # Path where the ordered matrices will be saved
email = # Email to be used for NEOS server

words = [
    'work',
    'position',
    'place',
    'change',
    'force',
    'point',
    'body',
    'name',
    'concern',
    'study',
    'paper',
    'front',
    'hand',
    'call',
    'arm',
    'floor',
    'production',
    'church',
    'side',
    'issue',
    'house',
    'instrument',
    'room',
    'model',
    'air',
    'stock',
    'stage',
    'capital',
    'bank',
    'field',
    'figure',
    'address',
    'impact',
    'office',
    'ground',
    'source',
    'article',
    'area',
    'section',
    'atmosphere',
    'birth',
    'authority',
    'response',
    'state',
    'performance',
    'connection',
    'property',
    'party',
    'title',
    'object',
    'lot',
    'interest',
    'policy',
    'duty',
    'matter',
    'division',
    'provision',
    'share',
    'nature',
    'security',
    'contribution',
    'school',
    'account',
    'capacity',
    'order',
    'expense',
    'service',
    'value',
    'claim',
    'assumption',
    'question',
    'cause',
    'form',
    'factor',
    'respect',
    'majority',
    'case',
    'girl',
    'vote',
    'growth',
    'press',
    'man',
    'practice',
    'idea',
    'age',
    'number',
    'job',
    'vehicle',
    'belief',
    'action',
    'book',
    'status',
    'fear',
    'money',
    'mile',
    'time',
    'life',
    'week',
    'faith',
    'test',
    'requirement',
    'answer',
    'note',
    'return',
    'need',
    'use',
    'show',
    'present',
    'design',
    'standard',
    'open',
    'right',
    'result',
    'close',
    'report',
    'support',
    'act',
    'end',
    'set',
    'public',
    'human',
    'whole',
    'individual',
    'future',
    'second',
    'hope',
    'base',
    'past',
    'fall',
    'cell',
]

processes = []
logfiles = []
os.makedirs(f"{output_path}/_logs/", exist_ok=True)
for word in words:
    # Order based on distances in provided matrix or based on Moran's I metric
    for order_type, order_type_flag in [("raw", "-r"), ("morans_i", "-m")]:
        os.makedirs(f"{output_path}/{word}/{order_type}/", exist_ok=True)

        logfile = open(f"{output_path}/_logs/log_{word}_{order_type}.txt", "w")
        logfiles.append(logfile)

        i = 0
        while i < 13:
            # We queue up only 9 words at a time to avoid NEOS server rate limiting
            if len(processes) < 9:
                tmpf = tempfile.TemporaryFile()
                p = subprocess.Popen([
                    exe_path, 
                    order_type_flag, 
                    "-e", email,
                    "-i", f"{source_matrix_path}/{word}/layer_{i}.in", 
                    "-o", f"{output_path}/{word}/{order_type}/layer_{i}.in", 
                    "-s", 
                    "-v", f"{output_path}/{word}/{order_type}/layer_{i}.bmp"
                ], stdout=tmpf)
                processes.append((word, i, p, tmpf, logfile))
                i += 1
                q = [(w, ii) for w, ii, _, _, _ in processes]
                print("current queue: ", q)
            else:
                for w, ii, p, tmpf, logf in processes:
                    poll = p.poll()
                    if poll is not None:
                        tmpf.seek(0)
                        logf.write(tmpf.read().decode("utf-8"))
                        logf.flush()
                        tmpf.close()
                        processes.remove((w, ii, p, tmpf, logf))

                        print(f"{w} layer {ii} finished")
                        break
                
                time.sleep(5)

# Wait for all processes to finish
for w, ii, p, tmpf, logf in processes:
    p.wait()
    tmpf.seek(0)
    logf.write(tmpf.read().decode("utf-8"))
    logf.flush()
    tmpf.close()

    print(f"{w} layer {ii} finished")

for logf in logfiles:
    logf.close()