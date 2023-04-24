import numpy as np
import xml.sax

# create an XMLReader
parser = xml.sax.make_parser()
# turn off namepsaces
parser.setFeature(xml.sax.handler.feature_namespaces, 0)
# override the default ContextHandler
parser.parse("Python/SAF/Rotationmatrix.xml")

print()

class HomogeneousTransform(xml.sax.ContentHandler):
    def __init__(self, matrix):
        self.matrix = matrix
        
    def multiply(self, other):
        new_matrix = np.dot(self.matrix, other.matrix)
        return new_matrix
    
    def transpose(self):
        new_matrix = np.transpose(self.matrix)
        return new_matrix
    
    def __str__(self):
        return str(self.matrix)

Bababooey = HomogeneousTransform("Python\SAF\Rotationmatrix.xml")
Bababooey.__str__()
print(Bababooey)
def is_proper_rotation_matrix(matrix):
    if matrix.shape == (2,2):
        transpose_condition = np.allclose(matrix, matrix.T)
        determinant_condition = np.isclose(np.linalg.det(matrix), 1.0)
        return transpose_condition and determinant_condition
    elif matrix.shape == (3,3):
        transpose_condition = np.allclose(matrix, matrix.T)
        determinant_condition = np.isclose(np.linalg.det(matrix), 1.0)
        return transpose_condition and determinant_condition
    else:
        return False