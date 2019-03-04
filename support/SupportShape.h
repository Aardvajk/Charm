#ifndef SUPPORTSHAPE_H
#define SUPPORTSHAPE_H

#include "maths/Maths.h"

class SupportProvider;
class CreateCommand;

void
createSupportShape(const Vec3 &pos, const SupportProvider &support, CreateCommand *command, int existingVertexCount);

#endif // SUPPORTSHAPE_H
