import Geometry3D as geometry
import sys

def parse_numbers (file_name):

    numbers = []
    with open(file_name) as file:
        for line in file:
            for x in line.split():
                numbers.append(float(x))

    n_triangles = int(numbers[0])
    numbers.pop(0)

    return numbers, n_triangles

def construct_points (numbers, n_triangles):

    points = []
    n_numbers = n_triangles * 9

    i = 0
    while i < n_numbers - 2:
        points.append(geometry.Point(numbers[i], numbers[i + 1], numbers[i + 2]))
        i += 3

    return points

def construct_triangles (points):

    triangles = []

    i = 0
    while i < len(points) - 2:
        triangles.append(geometry.ConvexPolygon((points[i], points[i + 1], points[i + 2])))
        i += 3

    return triangles

def intersect_all (triangles):

    intersecting_indexes = []
    for i in range(len(triangles) - 1):
        for j in range(i + 1, len(triangles)):
            if geometry.intersection(triangles[i], triangles[j]):
                if i not in intersecting_indexes:
                    intersecting_indexes.append(i)
                if j not in intersecting_indexes:
                    intersecting_indexes.append(j)

    return intersecting_indexes

def main ():

    test_name = sys.argv[1]
    numbers, n_triangles = parse_numbers (test_name)
    points = construct_points (numbers, n_triangles)
    triangles = construct_triangles (points)
    intersecting_indexes = intersect_all (triangles)
    intersecting_indexes.sort()

    print (*intersecting_indexes)

main()
