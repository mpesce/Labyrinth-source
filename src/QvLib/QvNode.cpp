/*
 * QvNode.cpp - Base class for all VRML nodes
 *
 * Part of Labyrinth VRML Browser
 * Copyright (c) 1995, The Community Company
 * Reconstructed from LABYRNTH.EXE (built March 4, 1995)
 */

#include "../../include/QvNode.h"
#include "../../include/QvFields.h"
#include <stdlib.h>
#include <string.h>

// Static member initialization
void* QvNode::nameDict = NULL;

/*
 * QvNode constructor
 */
QvNode::QvNode()
{
    name = NULL;
    refCount = 0;
}

/*
 * QvNode destructor
 */
QvNode::~QvNode()
{
    // Remove from name dictionary if named
    if (name != NULL) {
        removeName();
        free(name);
        name = NULL;
    }
}

/*
 * Set the DEF name for this node
 */
void QvNode::setName(const char* newName)
{
    if (name != NULL) {
        free(name);
    }

    if (newName != NULL) {
        name = (char*)malloc(strlen(newName) + 1);
        strcpy(name, newName);

        // Add to global name dictionary
        // TODO: Implement B-tree insertion
        // For now, simple implementation
    } else {
        name = NULL;
    }
}

/*
 * Get the DEF name for this node
 */
const char* QvNode::getName() const
{
    return name;
}

/*
 * Find a node by its DEF name (USE reference)
 */
QvNode* QvNode::findByName(const char* searchName)
{
    // TODO: Implement B-tree search
    // String from analysis: "Unknown reference \"%s\"" (line 718)
    // String from analysis: "Premature end of file after USE" (line 719)

    // For now, return NULL
    return NULL;
}

/*
 * Remove this node from the name dictionary
 *
 * Referenced in strings as "QvNode::removeName" (line 716)
 * Error: "Name \"%s\" (node %x) is not in dictionary" (line 717)
 */
void QvNode::removeName()
{
    if (name == NULL) {
        return;
    }

    // TODO: Implement B-tree removal
    // For now, no-op
}

/*
 * Get field by name
 */
QvField* QvNode::getField(const char* fieldName)
{
    QvFieldData* fd = getFieldData();
    if (fd == NULL) {
        return NULL;
    }

    return fd->getField(fieldName);
}

/*
 * Get number of fields
 */
int QvNode::getNumFields()
{
    QvFieldData* fd = getFieldData();
    if (fd == NULL) {
        return 0;
    }

    return fd->getNumFields();
}

/*
 * Increment reference count
 */
void QvNode::ref()
{
    refCount++;
}

/*
 * Decrement reference count
 */
void QvNode::unref()
{
    refCount--;
    if (refCount <= 0) {
        delete this;
    }
}

/*
 * Get reference count
 */
int QvNode::getRefCount() const
{
    return refCount;
}

/*
 * Read instance from input stream
 *
 * Handles DEF/USE and field parsing.
 *
 * Strings from analysis:
 * - "Premature end of file after DEF" (line 724)
 * - "No name given after " (line 723)
 * - "Invalid definition of %s" (line 722)
 */
BOOL QvNode::readInstance(QvInput* in)
{
    if (in == NULL) {
        return FALSE;
    }

    // Read opening brace
    char c;
    in->skipWhiteSpace();
    if (!in->read(&c) || c != '{') {
        return FALSE;
    }

    // Read fields until closing brace
    QvFieldData* fieldData = getFieldData();
    if (fieldData == NULL) {
        return FALSE;
    }

    while (TRUE) {
        in->skipWhiteSpace();

        // Check for closing brace
        if (!in->read(&c)) {
            return FALSE;
        }

        if (c == '}') {
            break;  // End of node
        }

        // Put character back
        // TODO: Implement pushback in QvInput

        // Read field name
        char fieldName[256];
        if (!in->readIdentifier(fieldName, sizeof(fieldName))) {
            return FALSE;
        }

        // Get field
        QvField* field = fieldData->getField(fieldName);
        if (field == NULL) {
            // Unknown field - error
            // "Unknown field \"%s\"" (line 693)
            return FALSE;
        }

        // Read field value
        if (!field->read(in, fieldName)) {
            // "Couldn't read value for field \"%s\"" (line 686)
            return FALSE;
        }
    }

    return TRUE;
}
