#!/bin/bash

# Release script for TB-303 Emulator
# Usage: ./release.sh [major|minor|patch] [release_notes]

set -e

VERSION_TYPE=${1:-patch}
RELEASE_NOTES=${2:-"Bug fixes and improvements"}

# Check if we're in a git repository
if ! git rev-parse --git-dir > /dev/null 2>&1; then
    echo "Error: Not in a git repository"
    exit 1
fi

# Check if working directory is clean
if ! git diff-index --quiet HEAD --; then
    echo "Error: Working directory is not clean. Please commit your changes first."
    exit 1
fi

# Get current version from CMakeLists.txt
CURRENT_VERSION=$(grep "project(TB303Emulator VERSION" CMakeLists.txt | sed 's/.*VERSION \([0-9.]*\).*/\1/')
echo "Current version: $CURRENT_VERSION"

# Split version into parts
IFS='.' read -r -a VERSION_PARTS <<< "$CURRENT_VERSION"
MAJOR=${VERSION_PARTS[0]}
MINOR=${VERSION_PARTS[1]}
PATCH=${VERSION_PARTS[2]}

# Increment version based on type
case $VERSION_TYPE in
    major)
        MAJOR=$((MAJOR + 1))
        MINOR=0
        PATCH=0
        ;;
    minor)
        MINOR=$((MINOR + 1))
        PATCH=0
        ;;
    patch)
        PATCH=$((PATCH + 1))
        ;;
    *)
        echo "Error: Invalid version type. Use major, minor, or patch."
        exit 1
        ;;
esac

NEW_VERSION="$MAJOR.$MINOR.$PATCH"
echo "New version: $NEW_VERSION"

# Confirm with user
read -p "Create release $NEW_VERSION? (y/N): " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Release cancelled."
    exit 0
fi

# Update version in CMakeLists.txt
sed -i.bak "s/project(TB303Emulator VERSION [0-9.]*/project(TB303Emulator VERSION $NEW_VERSION/" CMakeLists.txt
rm CMakeLists.txt.bak 2>/dev/null || true

# Commit version change
git add CMakeLists.txt
git commit -m "Bump version to $NEW_VERSION"

# Create and push tag
git tag -a "v$NEW_VERSION" -m "Release $NEW_VERSION

$RELEASE_NOTES"

echo "Created tag v$NEW_VERSION"
echo ""
echo "To complete the release:"
echo "1. Push the changes: git push origin main"
echo "2. Push the tag: git push origin v$NEW_VERSION"
echo "3. GitHub Actions will automatically build and create the release"
echo ""
echo "Or push both at once:"
echo "git push origin main --follow-tags"