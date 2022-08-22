#include "heightmap.h"

HeightMap::HeightMap(QString fileName, float heightMultiplier, float scalePerStep, int pixelsPerStep)
    : mScalePerStep(scalePerStep), mHeightMultiplier(heightMultiplier), mPixelsPerStep(pixelsPerStep)
{
    setVertices(CreateArraysFromHeightmap(fileName, heightMultiplier, scalePerStep, pixelsPerStep));
}

inline std::vector<Vertex> HeightMap::CreateArraysFromHeightmap(const QString &filepath, float multiplier, float scalePerStep, int pixelsPerStep)
{

    QImage img(filepath);

    mImg = img;//.scaledToHeight(200);

    std::vector<Vertex>mVertices;
    std::cout << pixelsPerStep << " " << mImg.height() << std::endl;

    for (int r = mImg.height()-pixelsPerStep; r > 0; r -= pixelsPerStep) {
        for (int c = 0; c < mImg.width()-pixelsPerStep; c += pixelsPerStep) {
            //int index = r*mImg.width() + c;
            //float avg = float(*(mImg.constBits() + index) + *(mImg.constBits() + index+1) + *(mImg.constBits() + index+2)) / 3.f / 255.f;

            // top left
            QVector3D vecTopLeft = {float(c) * scalePerStep, float(mImg.height()-r) * scalePerStep, (1-mImg.pixelColor(c, r).lightnessF()) * multiplier};

            // top right
            QVector3D vecTopRight = {float(c+pixelsPerStep) * scalePerStep, float(mImg.height()-r) * scalePerStep, (1-mImg.pixelColor(c+pixelsPerStep, r).lightnessF()) * multiplier};

            // bottom left
            QVector3D vecBottomLeft = {float(c) * scalePerStep, float(mImg.height()-r+pixelsPerStep) * scalePerStep, (1-mImg.pixelColor(c, r-pixelsPerStep).lightnessF()) * multiplier};

            // bottom right
            QVector3D vecBottomRight = {float(c+pixelsPerStep) * scalePerStep, float(mImg.height()-r+pixelsPerStep) * scalePerStep, (1-mImg.pixelColor(c+pixelsPerStep, r-pixelsPerStep).lightnessF()) * multiplier};

            QVector3D tri1normal = QVector3D::crossProduct(vecTopLeft-vecTopRight, vecTopLeft-vecBottomLeft);

            QVector3D tri2normal = QVector3D::crossProduct(vecBottomRight-vecBottomLeft, vecBottomRight-vecTopLeft);


            UV uvs = CalculateUV(0, mImg.width() * scalePerStep,0, mImg.height() * scalePerStep,  vecTopLeft.x(), vecTopLeft.y());

            mVertices.push_back(Vertex{vecTopLeft, tri1normal, uvs.u, uvs.v});
            uvs = CalculateUV(0, mImg.width() * scalePerStep,0, mImg.height() * scalePerStep,  vecBottomLeft.x(), vecBottomLeft.y());
            mVertices.push_back(Vertex{vecBottomLeft, tri1normal, uvs.u, uvs.v});
            uvs = CalculateUV(0, mImg.width() * scalePerStep,0, mImg.height() * scalePerStep,  vecTopRight.x(), vecTopRight.y());
            mVertices.push_back(Vertex{vecTopRight, tri1normal, uvs.u, uvs.v});

            mVertices.push_back(Vertex{vecTopRight, tri2normal, uvs.u, uvs.v});
            uvs = CalculateUV(0, mImg.width() * scalePerStep,0, mImg.height() * scalePerStep,  vecBottomLeft.x(), vecBottomLeft.y());
            mVertices.push_back(Vertex{vecBottomLeft, tri2normal, uvs.u, uvs.v});
            uvs = CalculateUV(0, mImg.width() * scalePerStep,0, mImg.height() * scalePerStep,  vecBottomRight.x(), vecBottomRight.y());
            mVertices.push_back(Vertex{vecBottomRight, tri2normal, uvs.u, uvs.v});

        }
    }

    return mVertices;

//    std::vector<float> values;
//    values.reserve(s.size() / (3+mImg.hasAlphaChannel()));

//    for (int i = 0; i < s.size(); i += 3 + mImg.hasAlphaChannel()) {
//        float avg = float(s[i] + s[i+1] + s[i+2]) / 3.f;
//        values.push_back(avg);
//    }


//    for (auto c : s) {
//        std::cout << (int)c << " ";
//    }

}

UV HeightMap::CalculateUV(float xMin, float xMax, float yMin, float yMax, float x, float y)
{
    return GraphMaker::CalculateUV(xMin, xMax, yMin, yMax, x, y);
}


void HeightMap::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0  );          // array buffer offset
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void HeightMap::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    //glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    //glDrawElements(mDrawMethod, mVertices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glBindVertexArray(0);
}

QVector3D HeightMap::GetNormal(float x, float y)
{
    return mCurrentNormal.normalized();
}

float HeightMap::GetHeight(float x, float y)
{
    QVector3D pos {x,y,0};
    pos = mMatrix.inverted().map(pos);
    x = pos.x();
    y = pos.y();
    int xInt = x / mScalePerStep;
    int yInt = mImg.height() - y / mScalePerStep;
    float height = 0;
    if (xInt + 2 > mImg.width()) {
        xInt = mImg.width() - 2;
    }
        if (yInt + 2 > mImg.height()) {
            yInt = mImg.height() - 2;
        }
    if (xInt < 0) xInt = 0;
    if (yInt < 0) yInt = 0;

    QVector3D p1{float((xInt) * mScalePerStep), float((mImg.height() - yInt -1) * mScalePerStep), (1-mImg.pixelColor(xInt, yInt + 1).lightnessF()) * mHeightMultiplier};
    QVector3D p2{float((xInt + 1) * mScalePerStep), float((mImg.height() - yInt - 1) * mScalePerStep), (1-mImg.pixelColor(xInt + 1, yInt + 1).lightnessF()) * mHeightMultiplier};
    QVector3D p3{float((xInt) * mScalePerStep), float((mImg.height() - yInt) * mScalePerStep), (1-mImg.pixelColor(xInt, yInt).lightnessF()) * mHeightMultiplier};

    QVector3D baryc = Barycentric({x, y, 0}, p1, p2, p3);
    if (isOverlappingTriangle(baryc, p1, p2, p3)) {
        height = GetBarycentricHeight(baryc, p1, p2, p3);
        mCurrentNormal = QVector3D::crossProduct((p2-p1), (p3-p2));
    } else {
        p1 = {float((xInt+1) * mScalePerStep), float((mImg.height() - yInt) * mScalePerStep), (1-mImg.pixelColor(xInt + 1, yInt).lightnessF()) * mHeightMultiplier};
        // 2 1 3
        baryc = Barycentric({x, y, 0}, p2, p1, p3);
        height = GetBarycentricHeight(baryc, p2, p1, p3);
        mCurrentNormal = QVector3D::crossProduct((p1-p2), (p3-p1));
    }
    pos.setZ(height);
    pos = mMatrix.map(pos);
    height = pos.z();
    return height;
}
