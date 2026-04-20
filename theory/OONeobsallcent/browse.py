import h5py
import argparse
import sys
import pandas as pd
# import os

def collect_datasets(h5_file, path = '', toprint = True):
    datasets = []
    
    for key in h5_file.keys():
        item = h5_file[key]
        full_path = f"{path}/{key}" if path else key
        spaces = full_path.count('/') * "  "
        if isinstance(item, h5py.Dataset):
            if toprint:
                print(f"{spaces} (d) {full_path}")
            datasets.append((full_path, item))
        elif isinstance(item, h5py.Group):
            if toprint:
                print(f"{spaces} \033[2m(G) {full_path}\033[0m")
            datasets.extend(collect_datasets(item, full_path, toprint))

    return datasets

def dataset_to_dict(das, obs):
    print(obs)
    dset = None
    for d in das:
        if d[0] == obs:
            dset = d[1]

    if dset is None:
        print("error: bad key")
        return dset

    data = dset[()]#.tolist()
    result = []
    ndim = data.ndim
    if ndim == 1:
        result.append(data.tolist())
    elif ndim == 2:
        for i in range(0, ndim):
            rr = []
            for d in data:
                d = d.tolist()[i]
                rr.append(d)
                
            result.append(rr)
    else:
        print("error: skim datasets > 2 dimensions")

    # df = pd.DataFrame(data)

    # print(result)
    return result

def main(input_file, obs = ''):
    print(input_file)
    das = []
    with h5py.File(input_file, 'r') as f:
        das = collect_datasets(f, '', (obs == ''))
        # for d in das:
        #     print(f"\033[2m{d}\033[0m")

        if obs:
            print()
            print(obs)
            # cent = dataset_to_dict(das, obs + "/cent")
            y = dataset_to_dict(das, obs + "/y")
            print([100 - x for x in y[0]])
            print([float(f"{x:.6g}") for x in y[1]])
            tot = dataset_to_dict(das, obs + "/dy/tot")
            print([float(f"{x:.6g}") for x in tot[0]])

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('input_file', help='Input HDF5 file path')
    args = parser.parse_args()
    main(args.input_file)
    main(args.input_file, 'multiplicitycharged')
    main(args.input_file, 'numpart')
